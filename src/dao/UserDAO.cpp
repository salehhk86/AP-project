#include "UserDAO.hpp"
#include <iostream>
using namespace std;

// helper
unique_ptr<User> UserDAO::CreateUserFromRow(sqlite3_stmt *stmt)
{
    long id = sqlite3_column_int64(stmt, 0);
    string username = safeStr(stmt, 1);
    string password = safeStr(stmt, 2);
    string phone = safeStr(stmt, 3);
    string address = safeStr(stmt, 4);
    Role role = static_cast<Role>(sqlite3_column_int(stmt, 5));
    bool isActive = sqlite3_column_int(stmt, 6) != 0;
    string created = safeStr(stmt, 7);
    long restaurantId = sqlite3_column_int64(stmt, 8); // NULL → 0

    unique_ptr<User> user = nullptr;

    if (role == Role::Customer)
        user = make_unique<Customer>(id, username, password, phone, address, created);
    else if (role == Role::Admin)
        user = make_unique<Admin>(id, username, password, phone, address, created);
    else if (role == Role::RestaurantManager)
    {
        auto rm = make_unique<RestaurantManager>(id, username, password, phone, address, created, restaurantId);
        user = move(rm);
    }

    if (user)
        user->SetIs_Active(isActive);

    return user;
}

string UserDAO::safeStr(sqlite3_stmt *stmt, int col)
{
    const unsigned char *val = sqlite3_column_text(stmt, col);
    return val ? reinterpret_cast<const char *>(val) : "";
}

UserDAO::UserDAO(sqlite3 *database) : db(database) {}

// create
void UserDAO::CreateTable()
{
    const char *sql = "CREATE TABLE IF NOT EXISTS USER ("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "USERNAME TEXT NOT NULL UNIQUE, "
                      "PASSWORD TEXT NOT NULL, "
                      "PHONE_NUMBER TEXT, "
                      "ADDRESS TEXT, "
                      "ROLE INTEGER, "
                      "IS_ACTIVE INTEGER, "
                      "CREATED_AT TEXT, "
                      "RESTAURANT_ID INTEGER);";

    char *errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        cerr << "CreateTable USER failed: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

bool UserDAO::Create(const User &user)
{
    // INSERT
    const char *sql =
        "INSERT INTO USER "
        "(USERNAME, PASSWORD, PHONE_NUMBER, ADDRESS, ROLE, IS_ACTIVE, CREATED_AT, RESTAURANT_ID) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, user.GetUserName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user.GetPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.GetPhone_Number().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, user.GetAddress().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, static_cast<int>(user.GetRole()));
    sqlite3_bind_int(stmt, 6, user.GetIs_Active() ? 1 : 0);
    sqlite3_bind_text(stmt, 7, user.GetCreated_At().c_str(), -1, SQLITE_TRANSIENT);

    const RestaurantManager *rm = dynamic_cast<const RestaurantManager *>(&user);
    if (rm)
        sqlite3_bind_int64(stmt, 8, rm->GetRestaurantId());
    else
        sqlite3_bind_null(stmt, 8);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

// read
unique_ptr<User> UserDAO::ReadById(long id)
{
    // SELECT
    const char *sql =
        "SELECT ID, USERNAME, PASSWORD, PHONE_NUMBER, ADDRESS, ROLE, IS_ACTIVE, CREATED_AT, RESTAURANT_ID "
        "FROM USER WHERE ID = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_int64(stmt, 1, id);

    unique_ptr<User> user = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        user = CreateUserFromRow(stmt);

    sqlite3_finalize(stmt);
    return user;
}

unique_ptr<User> UserDAO::ReadByUsername(const string &username)
{
    // SELECT
    const char *sql =
        "SELECT ID, USERNAME, PASSWORD, PHONE_NUMBER, ADDRESS, ROLE, IS_ACTIVE, CREATED_AT, RESTAURANT_ID "
        "FROM USER WHERE USERNAME = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    unique_ptr<User> user = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        user = CreateUserFromRow(stmt);

    sqlite3_finalize(stmt);
    return user;
}

unique_ptr<User> UserDAO::ReadByUsernameForLogin(const string &username)
{
    const char *sql =
        "SELECT ID, USERNAME, PASSWORD, PHONE_NUMBER, ADDRESS, ROLE, IS_ACTIVE, CREATED_AT, RESTAURANT_ID "
        "FROM USER WHERE USERNAME = ? AND IS_ACTIVE = 1;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    unique_ptr<User> user = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        user = CreateUserFromRow(stmt);

    sqlite3_finalize(stmt);
    return user;
}

vector<unique_ptr<User>> UserDAO::ReadAll()
{
    vector<unique_ptr<User>> list;
    // SELECT
    const char *sql =
        "SELECT ID, USERNAME, PASSWORD, PHONE_NUMBER, ADDRESS, ROLE, IS_ACTIVE, CREATED_AT, RESTAURANT_ID FROM USER;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return list;

    while (sqlite3_step(stmt) == SQLITE_ROW)
        list.push_back(CreateUserFromRow(stmt));

    sqlite3_finalize(stmt);
    return list;
}

// update
bool UserDAO::Update(const User &user)
{
    // UPDATE
    const char *sql =
        "UPDATE USER SET USERNAME=?, PASSWORD=?, PHONE_NUMBER=?, ADDRESS=?, "
        "ROLE=?, IS_ACTIVE=?, CREATED_AT=?, RESTAURANT_ID=? WHERE ID=?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, user.GetUserName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user.GetPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.GetPhone_Number().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, user.GetAddress().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, static_cast<int>(user.GetRole()));
    sqlite3_bind_int(stmt, 6, user.GetIs_Active() ? 1 : 0);
    sqlite3_bind_text(stmt, 7, user.GetCreated_At().c_str(), -1, SQLITE_TRANSIENT);

    const RestaurantManager *rm = dynamic_cast<const RestaurantManager *>(&user);
    if (rm)
        sqlite3_bind_int64(stmt, 8, rm->GetRestaurantId());
    else
        sqlite3_bind_null(stmt, 8);

    sqlite3_bind_int64(stmt, 9, user.GetId());

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

// delete
bool UserDAO::Delete(long id)
{
    // DELETE
    const char *sql = "DELETE FROM USER WHERE ID=?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int64(stmt, 1, id);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success && sqlite3_changes(db) > 0;
}
