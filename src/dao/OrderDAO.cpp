#include "OrderDAO.hpp"
#include <iostream>

using namespace std;

OrderDAO::OrderDAO(sqlite3 *database) : db(database) {}

// helper
vector<Order::OrderLine> OrderDAO::GetLines(long long orderId)
{
    const char *sql =
        "SELECT ITEM_ID, QUANTITY, UNIT_PRICE "
        "FROM ORDER_LINES WHERE ORDER_ID = ?;";

    sqlite3_stmt *stmt;
    vector<Order::OrderLine> lines;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return lines;

    sqlite3_bind_int64(stmt, 1, orderId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Order::OrderLine line;
        line.itemId = sqlite3_column_int64(stmt, 0);
        line.quantity = sqlite3_column_int(stmt, 1);
        line.unitPrice = sqlite3_column_double(stmt, 2);

        lines.push_back(line);
    }

    sqlite3_finalize(stmt);
    return lines;
}

const char *OrderDAO::StatusToString(OrderStatus status)
{
    switch (status)
    {
    case OrderStatus::Pending:
        return "Pending";
    case OrderStatus::Preparing:
        return "Preparing";
    case OrderStatus::ReadyToSend:
        return "ReadyToSend";
    case OrderStatus::Delivered:
        return "Delivered";
    case OrderStatus::Cancelled:
        return "Cancelled";
    default:
        return "Pending";
    }
}

OrderStatus OrderDAO::StringToStatus(const string &status)
{
    if (status == "Preparing")
        return OrderStatus::Preparing;
    if (status == "ReadyToSend")
        return OrderStatus::ReadyToSend;
    if (status == "Delivered")
        return OrderStatus::Delivered;
    if (status == "Cancelled")
        return OrderStatus::Cancelled;
    return OrderStatus::Pending;
}

// create
void OrderDAO::CreateTables()
{
    const char *orderTable = R"(
        CREATE TABLE IF NOT EXISTS ORDERS(
            ORDER_ID INTEGER PRIMARY KEY AUTOINCREMENT,
            CUSTOMER_ID INTEGER NOT NULL,
            RESTAURANT_ID INTEGER NOT NULL,
            STATUS TEXT NOT NULL,
            CREATED_AT TEXT NOT NULL
        );
    )";

    const char *orderLinesTable = R"(
        CREATE TABLE IF NOT EXISTS ORDER_LINES(
            ORDER_ID INTEGER NOT NULL,
            ITEM_ID INTEGER NOT NULL,
            QUANTITY INTEGER NOT NULL,
            UNIT_PRICE REAL NOT NULL,
            PRIMARY KEY (ORDER_ID, ITEM_ID),
            FOREIGN KEY (ORDER_ID) REFERENCES ORDERS(ORDER_ID) ON DELETE CASCADE
        );
    )";

    char *errMsg = nullptr;

    if (sqlite3_exec(db, orderTable, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        cerr << "Create ORDERS table failed: " << errMsg << endl;
        sqlite3_free(errMsg);
    }

    if (sqlite3_exec(db, orderLinesTable, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        cerr << "Create ORDER_LINES table failed: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

bool OrderDAO::Create(const Order &order)
{
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    const char *sql =
        "INSERT INTO ORDERS (CUSTOMER_ID, RESTAURANT_ID, STATUS, CREATED_AT) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    sqlite3_bind_int64(stmt, 1, order.GetCustomerId());
    sqlite3_bind_int64(stmt, 2, order.GetRestaurantId());
    sqlite3_bind_text(stmt, 3, StatusToString(order.GetStatus()), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, order.GetCreatedAt().c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    sqlite3_finalize(stmt);
    long long orderId = sqlite3_last_insert_rowid(db);

    const char *lineSql =
        "INSERT INTO ORDER_LINES (ORDER_ID, ITEM_ID, QUANTITY, UNIT_PRICE) "
        "VALUES (?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, lineSql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    for (const auto &line : order.GetLines())
    {
        sqlite3_reset(stmt);
        sqlite3_bind_int64(stmt, 1, orderId);
        sqlite3_bind_int64(stmt, 2, line.itemId);
        sqlite3_bind_int(stmt, 3, line.quantity);
        sqlite3_bind_double(stmt, 4, line.unitPrice);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    return true;
}

// read
unique_ptr<Order> OrderDAO::ReadById(long long orderId)
{
    const char *sql =
        "SELECT CUSTOMER_ID, RESTAURANT_ID, STATUS, CREATED_AT "
        "FROM ORDERS WHERE ORDER_ID = ?;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_int64(stmt, 1, orderId);

    unique_ptr<Order> order = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        long long customerId = sqlite3_column_int64(stmt, 0);
        long long restaurantId = sqlite3_column_int64(stmt, 1);

        const char *statusTxt = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        const char *dateTxt = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

        string statusStr = statusTxt ? statusTxt : "Pending";
        string createdAt = dateTxt ? dateTxt : "";

        order = make_unique<Order>(orderId, customerId, restaurantId, StringToStatus(statusStr), createdAt);

        auto lines = GetLines(orderId);
        for (const auto &l : lines)
            order->AddLine(l.itemId, l.quantity, l.unitPrice);
    }

    sqlite3_finalize(stmt);
    return order;
}

vector<unique_ptr<Order>> OrderDAO::ReadByCustomer(long long customerId)
{
    const char *sql = "SELECT ORDER_ID FROM ORDERS WHERE CUSTOMER_ID = ?;";
    sqlite3_stmt *stmt;
    vector<unique_ptr<Order>> orders;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return orders;

    sqlite3_bind_int64(stmt, 1, customerId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        long long orderId = sqlite3_column_int64(stmt, 0);
        auto order = ReadById(orderId);
        if (order)
            orders.push_back(move(order));
    }

    sqlite3_finalize(stmt);
    return orders;
}

// update
bool OrderDAO::UpdateStatus(long long orderId, OrderStatus newStatus)
{
    const char *sql = "UPDATE ORDERS SET STATUS = ? WHERE ORDER_ID = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, StatusToString(newStatus), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 2, orderId);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success && sqlite3_changes(db) > 0;
}

// delete
bool OrderDAO::Delete(long long orderId)
{
    // Enable foreign key support might be needed for cascading delete
    // OR manually delete lines:
    sqlite3_exec(db, "DELETE FROM ORDER_LINES WHERE ORDER_ID = ?;", nullptr, nullptr, nullptr); // Simple approach

    const char *sql = "DELETE FROM ORDERS WHERE ORDER_ID = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int64(stmt, 1, orderId);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success && sqlite3_changes(db) > 0;
}
