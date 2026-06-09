#include "RestaurantDAO.hpp"

using namespace std;

RestaurantDAO::RestaurantDAO(sqlite3 *database) : db(database) {}

// create
void RestaurantDAO::CreateTable()
{
    const char *sql = "CREATE TABLE IF NOT EXISTS RESTAURANT ("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT, " // id is unique
                      "NAME TEXT NOT NULL, "
                      "ADDRESS TEXT, "
                      "IS_ACTIVE INTEGER, " // true = 1 , false = 0
                      "PHONE_NUMBER TEXT, "
                      "DESCRIPTION TEXT);";

    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
}

bool RestaurantDAO::Create(const Restaurant &restaurant)
{
    // INSERT
    const char *sql = "INSERT INTO RESTAURANT (NAME, ADDRESS, IS_ACTIVE, PHONE_NUMBER, DESCRIPTION) VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, restaurant.GetName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, restaurant.GetAddress().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, restaurant.GetIsActive() ? 1 : 0);
    sqlite3_bind_text(stmt, 4, restaurant.GetNumber().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, restaurant.GetDescription().c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE); // execute the command
    sqlite3_finalize(stmt);                             // free memory
    return success;
}

// read
unique_ptr<Restaurant> RestaurantDAO::ReadById(long long id)
{
    // SELECT
    const char *sql = "SELECT ID, NAME, ADDRESS, IS_ACTIVE, PHONE_NUMBER, DESCRIPTION FROM RESTAURANT WHERE ID = ?;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_int64(stmt, 1, id);

    unique_ptr<Restaurant> restaurant = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char *address = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        const char *phone = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        const char *desc = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));

        restaurant = make_unique<Restaurant>(
            sqlite3_column_int64(stmt, 0), // ID
            name ? name : "",
            address ? address : "",
            sqlite3_column_int(stmt, 3) != 0,
            phone ? phone : "",
            desc ? desc : "");
    }

    sqlite3_finalize(stmt);
    return restaurant;
}

vector<unique_ptr<Restaurant>> RestaurantDAO::ReadAll()
{
    vector<unique_ptr<Restaurant>> list;

    const char *sql = "SELECT ID, NAME, ADDRESS, IS_ACTIVE, PHONE_NUMBER, DESCRIPTION FROM RESTAURANT;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return list;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char *address = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        const char *phone = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        const char *desc = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));

        auto restaurant = make_unique<Restaurant>(
            sqlite3_column_int64(stmt, 0),
            name ? name : "",
            address ? address : "",
            sqlite3_column_int(stmt, 3) != 0,
            phone ? phone : "",
            desc ? desc : "");

        list.push_back(move(restaurant)); // transfer ownership
    }

    sqlite3_finalize(stmt);
    return list;
}

// update
bool RestaurantDAO::Update(const Restaurant &restaurant)
{
    const char *sql = "UPDATE RESTAURANT SET NAME=?, ADDRESS=?, IS_ACTIVE=?, PHONE_NUMBER=?, DESCRIPTION=? WHERE ID=?;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, restaurant.GetName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, restaurant.GetAddress().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, restaurant.GetIsActive() ? 1 : 0);
    sqlite3_bind_text(stmt, 4, restaurant.GetNumber().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, restaurant.GetDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 6, restaurant.GetId());

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success && sqlite3_changes(db) > 0;
}

// delete
bool RestaurantDAO::Delete(long long id)
{
    const char *sql = "DELETE FROM RESTAURANT WHERE ID = ?;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int64(stmt, 1, id);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success && sqlite3_changes(db) > 0;
}
