#include "UserDAO.hpp"

using namespace std;

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
                      "CREATED_AT TEXT);";

    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
}

bool UserDAO::Create(const User &user)
{
    const char *sql =
        "INSERT INTO USER "
        "(USERNAME, PASSWORD, PHONE_NUMBER, ADDRESS, ROLE, IS_ACTIVE, CREATED_AT) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);";

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

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}

// factory
unique_ptr<User> UserDAO::CreateUserFromRow(sqlite3_stmt *stmt)
{
    long id = sqlite3_column_int64(stmt, 0);

    string username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    string password = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    string phone = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
    string address = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    Role role = static_cast<Role>(sqlite3_column_int(stmt, 5));
    bool isActive = sqlite3_column_int(stmt, 6) != 0;
    string created = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));

    unique_ptr<User> user = nullptr;

    if (role == Role::Customer)
    {
        user = make_unique<Customer>(id, username, password, phone, address, created);
    }
    else if (role == Role::Admin)
    {
        user = make_unique<Admin>(id, username, password, phone, address, created);
    }
    else if (role == Role::RestaurantManager)
    {
        user = make_unique<RestaurantManager>(id, username, password, phone, address, created);
    }

    if (user)
        user->SetIs_Active(isActive);

    return user;
}

// read
unique_ptr<User> UserDAO::ReadById(long id)
{
    const char *sql =
        "SELECT ID, USERNAME, PASSWORD, PHONE_NUMBER, ADDRESS, ROLE, IS_ACTIVE, CREATED_AT "
        "FROM USER WHERE ID = ?;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_int64(stmt, 1, id);

    unique_ptr<User> user = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        user = CreateUserFromRow(stmt);
    }

    sqlite3_finalize(stmt);
    return user;
}

unique_ptr<User> UserDAO::ReadByUsername(const string &username)
{
    const char *sql =
        "SELECT ID, USERNAME, PASSWORD, PHONE_NUMBER, ADDRESS, ROLE, IS_ACTIVE, CREATED_AT "
        "FROM USER WHERE USERNAME = ?;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    unique_ptr<User> user = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        user = CreateUserFromRow(stmt);
    }

    sqlite3_finalize(stmt);
    return user;
}

vector<unique_ptr<User>> UserDAO::ReadAll()
{
    vector<unique_ptr<User>> list;

    const char *sql =
        "SELECT ID, USERNAME, PASSWORD, PHONE_NUMBER, ADDRESS, ROLE, IS_ACTIVE, CREATED_AT FROM USER;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return list;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        auto user = CreateUserFromRow(stmt);
        list.push_back(move(user));
    }

    sqlite3_finalize(stmt);
    return list;
}

// update
bool UserDAO::Update(const User &user)
{
    const char *sql =
        "UPDATE USER SET USERNAME=?, PASSWORD=?, PHONE_NUMBER=?, ADDRESS=?, ROLE=?, IS_ACTIVE=?, CREATED_AT=? WHERE ID=?;";

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
    sqlite3_bind_int64(stmt, 8, user.GetId());

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}

// delete
bool UserDAO::Delete(long id)
{
    const char *sql = "DELETE FROM USER WHERE ID=?;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int64(stmt, 1, id);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}
