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
                      "IS_ACTIVE INTEGER, " // true 1 , false 0
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

    bool success = (sqlite3_step(stmt) == SQLITE_DONE); // do the taxt
    sqlite3_finalize(stmt);                             // free the src and memorys
    return success;
}

// read
unique_ptr<Restaurant> RestaurantDAO::ReadById(long id)
{
    // SELECT
    const char *sql = "SELECT ID, NAME, ADDRESS, IS_ACTIVE, PHONE_NUMBER, DESCRIPTION FROM RESTAURANT WHERE ID = ?;"; // ? for the security

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;
    // data base with SELECT will find the right restaurant (no need for(;;))
    sqlite3_bind_int64(stmt, 1, id); // for (?)

    unique_ptr<Restaurant> restaurant = nullptr;

    // STEP = do the taxt
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        // rows start from 0 like array
        restaurant = make_unique<Restaurant>(
            sqlite3_column_int64(stmt, 0),                                // ID
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)), // Name
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)), // Address
            sqlite3_column_int(stmt, 3) != 0,                             // IsActive (تبدیل int به bool)
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)), // Number
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5))  // Description
            // sqlite3 will return unsigned char* but we need char* or string so we use reiterpret_cast<const char*>
        );
    }
    sqlite3_finalize(stmt); // free the src and memorys
    return restaurant;
}

vector<unique_ptr<Restaurant>> RestaurantDAO::ReadAll()
{
    vector<unique_ptr<Restaurant>> list;
    // SELECT
    const char *sql = "SELECT ID, NAME, ADDRESS, IS_ACTIVE, PHONE_NUMBER, DESCRIPTION FROM RESTAURANT;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return list;

    while (sqlite3_step(stmt) == SQLITE_ROW) // while there is a row
    {
        auto restaurant = make_unique<Restaurant>(
            sqlite3_column_int64(stmt, 0),                                // ID
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)), // Name
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)), // Address
            sqlite3_column_int(stmt, 3) != 0,                             // IsActive (int to bool)
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)), // Number
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5))  // Description
        );
        list.push_back(move(restaurant)); // move the ownership
    }
    sqlite3_finalize(stmt); // free the memorys
    return list;
}

// update
bool RestaurantDAO::Update(const Restaurant &restaurant)
{
    // update
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

    bool success = (sqlite3_step(stmt) == SQLITE_DONE); // do the taxt
    sqlite3_finalize(stmt);                           // free the memory
    return success;
}

bool RestaurantDAO::Delete(long id)
{
    // DELETE
    const char *sql = "DELETE FROM RESTAURANT WHERE ID = ?;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;
        
    sqlite3_bind_int64(stmt, 1, id);

    bool succes = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt); // free src
    return succes;
}