#include "ItemDAO.hpp"
#include "Food.hpp"
#include "Drink.hpp"
#include "OtherItem.hpp"

using namespace std;

ItemDAO::ItemDAO(sqlite3 *database) : db(database) {}

// create
unique_ptr<Item> ItemDAO::CreateItemFromRow(sqlite3_stmt *stmt)
{
    long id = sqlite3_column_int64(stmt, 0);
    // col 1 = RESTAURANT_ID (skip)

    const unsigned char *nameText = sqlite3_column_text(stmt, 2);
    const unsigned char *descText = sqlite3_column_text(stmt, 3);

    string name = nameText ? reinterpret_cast<const char *>(nameText) : "";
    string desc = descText ? reinterpret_cast<const char *>(descText) : "";

    double price = sqlite3_column_double(stmt, 4);
    double tax = sqlite3_column_double(stmt, 5);
    bool status = sqlite3_column_int(stmt, 6) != 0;
    int type = sqlite3_column_int(stmt, 7);

    unique_ptr<Item> item;

    switch (static_cast<ItemType>(type))
    {
    case ItemType::Food:
    {
        float calories = (float)sqlite3_column_double(stmt, 8);
        bool vegan = sqlite3_column_int(stmt, 9) != 0;
        int prep = sqlite3_column_int(stmt, 10);
        item = make_unique<Food>(id, name, desc, price, tax, calories, vegan, prep);
        break;
    }
    case ItemType::Drink:
    {
        int size = sqlite3_column_int(stmt, 11);
        item = make_unique<Drink>(id, name, desc, price, tax, static_cast<DrinkSize>(size));
        break;
    }
    default:
        item = make_unique<OtherItem>(id, name, desc, price, tax);
        break;
    }

    item->SetStatus(status);
    return item;
}

bool ItemDAO::CreateTable()
{
    const char *sql = R"(
        CREATE TABLE IF NOT EXISTS ITEM (
            ID            INTEGER PRIMARY KEY AUTOINCREMENT,
            RESTAURANT_ID INTEGER NOT NULL,
            NAME          TEXT    NOT NULL,
            DESCRIPTION   TEXT,
            PRICE         REAL    NOT NULL,
            TAX           REAL    NOT NULL DEFAULT 0,
            STATUS        INTEGER NOT NULL DEFAULT 1,
            TYPE          INTEGER NOT NULL,
            CALORIES      REAL,
            IS_VEGAN      INTEGER,
            PREP_TIME     INTEGER,
            DRINK_SIZE    INTEGER,
            FOREIGN KEY (RESTAURANT_ID) REFERENCES RESTAURANT(ID)
        );
    )";
    char *err = nullptr;
    bool ok = sqlite3_exec(db, sql, nullptr, nullptr, &err) == SQLITE_OK;
    if (!ok)
    {
        cerr << "CreateTable error: " << err << "\n";
        sqlite3_free(err);
    }
    return ok;
}

bool ItemDAO::Create(const Item &item, long restaurantId)
{
    const char *sql =
        "INSERT INTO ITEM "
        "(RESTAURANT_ID, NAME, DESCRIPTION, PRICE, TAX, STATUS, TYPE, "
        "CALORIES, IS_VEGAN, PREP_TIME, DRINK_SIZE) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int64(stmt, 1, restaurantId);
    sqlite3_bind_text(stmt, 2, item.GetName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, item.GetDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, item.GetPrice());
    sqlite3_bind_double(stmt, 5, item.GetTax());
    sqlite3_bind_int(stmt, 6, item.GetStatus() ? 1 : 0);
    sqlite3_bind_int(stmt, 7, static_cast<int>(item.GetType()));

    // default nulls for type-specific columns
    sqlite3_bind_null(stmt, 8);
    sqlite3_bind_null(stmt, 9);
    sqlite3_bind_null(stmt, 10);
    sqlite3_bind_null(stmt, 11);

    if (const Food *food = dynamic_cast<const Food *>(&item))
    {
        sqlite3_bind_double(stmt, 8, food->GetCalories());
        sqlite3_bind_int(stmt, 9, food->GetIsVegan() ? 1 : 0);
        sqlite3_bind_int(stmt, 10, food->GetPreparationTimeMinutes());
    }
    else if (const Drink *drink = dynamic_cast<const Drink *>(&item))
    {
        sqlite3_bind_int(stmt, 11, static_cast<int>(drink->GetDrinkSize()));
    }

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

// read
unique_ptr<Item> ItemDAO::ReadById(long id)
{
    //SELECT
    const char *sql =
        "SELECT ID, RESTAURANT_ID, NAME, DESCRIPTION, "
        "PRICE, TAX, STATUS, TYPE, CALORIES, "
        "IS_VEGAN, PREP_TIME, DRINK_SIZE "
        "FROM ITEM WHERE ID=?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_int64(stmt, 1, id);

    unique_ptr<Item> item = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        item = CreateItemFromRow(stmt);

    sqlite3_finalize(stmt);
    return item;
}

vector<unique_ptr<Item>> ItemDAO::ReadByRestaurantId(long restaurantId)
{
    vector<unique_ptr<Item>> items;
    //SELECT
    const char *sql =
        "SELECT ID, RESTAURANT_ID, NAME, DESCRIPTION, "
        "PRICE, TAX, STATUS, TYPE, CALORIES, "
        "IS_VEGAN, PREP_TIME, DRINK_SIZE "
        "FROM ITEM WHERE RESTAURANT_ID=?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return items;

    sqlite3_bind_int64(stmt, 1, restaurantId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
        items.push_back(CreateItemFromRow(stmt));

    sqlite3_finalize(stmt);
    return items;
}

// update
bool ItemDAO::Update(const Item &item)
{
    // UPDATE
    const char *sql =
        "UPDATE ITEM SET "
        "NAME=?, DESCRIPTION=?, PRICE=?, TAX=?, STATUS=?, TYPE=?, "
        "CALORIES=?, IS_VEGAN=?, PREP_TIME=?, DRINK_SIZE=? "
        "WHERE ID=?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, item.GetName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, item.GetDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, item.GetPrice());
    sqlite3_bind_double(stmt, 4, item.GetTax());
    sqlite3_bind_int(stmt, 5, item.GetStatus() ? 1 : 0);
    sqlite3_bind_int(stmt, 6, static_cast<int>(item.GetType()));

    sqlite3_bind_null(stmt, 7);
    sqlite3_bind_null(stmt, 8);
    sqlite3_bind_null(stmt, 9);
    sqlite3_bind_null(stmt, 10);

    if (const Food *food = dynamic_cast<const Food *>(&item))
    {
        sqlite3_bind_double(stmt, 7, food->GetCalories());
        sqlite3_bind_int(stmt, 8, food->GetIsVegan() ? 1 : 0);
        sqlite3_bind_int(stmt, 9, food->GetPreparationTimeMinutes());
    }
    else if (const Drink *drink = dynamic_cast<const Drink *>(&item))
    {
        sqlite3_bind_int(stmt, 10, static_cast<int>(drink->GetDrinkSize()));
    }

    sqlite3_bind_int64(stmt, 11, item.GetId());

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

// delete
bool ItemDAO::Delete(long id)
{
    // DELETE
    const char *sql = "DELETE FROM ITEM WHERE ID=?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int64(stmt, 1, id);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}
