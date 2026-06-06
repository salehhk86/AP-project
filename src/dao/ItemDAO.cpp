#include "ItemDAO.hpp"

using namespace std;

ItemDAO::ItemDAO(sqlite3 *database) : db(database) {}


unique_ptr<Item> ItemDAO::CreateItemFromRow(sqlite3_stmt *stmt)
{
    long id = sqlite3_column_int64(stmt, 0);

    const unsigned char *nameText = sqlite3_column_text(stmt, 1);
    const unsigned char *descText = sqlite3_column_text(stmt, 2);

    string name = nameText ? reinterpret_cast<const char *>(nameText) : "";
    string desc = descText ? reinterpret_cast<const char *>(descText) : "";

    double price = sqlite3_column_double(stmt, 3);
    double tax = sqlite3_column_double(stmt, 4);
    bool status = sqlite3_column_int(stmt, 5) != 0;
    int type = sqlite3_column_int(stmt, 6);

    unique_ptr<Item> item;

    switch (static_cast<ItemType>(type))
    {
    case ItemType::Food:
    {
        float calories = sqlite3_column_double(stmt, 7);
        bool vegan = sqlite3_column_int(stmt, 8) != 0;
        int prep = sqlite3_column_int(stmt, 9);

        item = make_unique<Food>(id, name, desc, price, tax, calories, vegan, prep);
        break;
    }

    case ItemType::Drink:
    {
        int size = sqlite3_column_int(stmt, 10);

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

void ItemDAO::CreateTable()
{
    const char *sql =
        "CREATE TABLE IF NOT EXISTS ITEM ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "NAME TEXT NOT NULL, "
        "DESCRIPTION TEXT, "
        "PRICE REAL, "
        "TAX REAL, "
        "STATUS INTEGER, "
        "TYPE INTEGER NOT NULL, "
        "CALORIES REAL, "
        "IS_VEGAN INTEGER, "
        "PREP_TIME INTEGER, "
        "DRINK_SIZE INTEGER);";

    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
}

bool ItemDAO::Create(const Item &item)
{
    // INSERT
    const char *sql =
        "INSERT INTO ITEM "
        "(NAME, DESCRIPTION, PRICE, TAX, STATUS, TYPE, "
        "CALORIES, IS_VEGAN, PREP_TIME, DRINK_SIZE) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, item.GetName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, item.GetDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, item.GetPrice());
    sqlite3_bind_double(stmt, 4, item.GetTax());
    sqlite3_bind_int(stmt, 5, item.GetStatus() ? 1 : 0);
    sqlite3_bind_int(stmt, 6, static_cast<int>(item.GetType()));

    // default nulls
    sqlite3_bind_null(stmt, 7);
    sqlite3_bind_null(stmt, 8);
    sqlite3_bind_null(stmt, 9);
    sqlite3_bind_null(stmt, 10);

    if (item.GetType() == ItemType::Food)
    {
        const Food &food = dynamic_cast<const Food &>(item);
        sqlite3_bind_double(stmt, 7, food.GetCalories());
        sqlite3_bind_int(stmt, 8, food.GetIsVegan() ? 1 : 0);
        sqlite3_bind_int(stmt, 9, food.GetPreparationTimeMinutes());
    }
    else if (item.GetType() == ItemType::Drink)
    {
        const Drink &drink = dynamic_cast<const Drink &>(item);
        sqlite3_bind_int(stmt, 10, static_cast<int>(drink.GetDrinkSize()));
    }

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}

// read
unique_ptr<Item> ItemDAO::ReadById(long id)
{
    const char *sql = "SELECT * FROM ITEM WHERE ID=?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_int64(stmt, 1, id);

    unique_ptr<Item> item = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        item = CreateItemFromRow(stmt);
    }

    sqlite3_finalize(stmt);
    return item;
}

vector<unique_ptr<Item>> ItemDAO::ReadAll()
{
    vector<unique_ptr<Item>> items;

    const char *sql = "SELECT * FROM ITEM;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return items;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        items.push_back(CreateItemFromRow(stmt));
    }

    sqlite3_finalize(stmt);
    return items;
}

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

    switch (item.GetType())
    {
    case ItemType::Food:
    {
        const Food &food = dynamic_cast<const Food &>(item);
        sqlite3_bind_double(stmt, 7, food.GetCalories());
        sqlite3_bind_int(stmt, 8, food.GetIsVegan() ? 1 : 0);
        sqlite3_bind_int(stmt, 9, food.GetPreparationTimeMinutes());
        break;
    }

    case ItemType::Drink:
    {
        const Drink &drink = dynamic_cast<const Drink &>(item);
        sqlite3_bind_int(stmt, 10, static_cast<int>(drink.GetDrinkSize()));
        break;
    }

    default:
    {
        break;
    }
    }

    sqlite3_bind_int64(stmt, 11, item.GetId());

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
} // dynamic cast is for safty and the polymorthis

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
