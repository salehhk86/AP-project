#include "UserDAO.hpp"

using namespace std;

UserDAO::UserDAO(sqlite3 *database) : db(database) {}

void UserDAO::CreateTable()
{
    const char *sql = "CREATE TABLE IF NOT EXISTS USER ("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT, " // id is unique and made by database
                      "USERNAME TEXT NOT NULL UNIQUE, "
                      "PASSWORD TEXT NOT NULL, "
                      "PHONE_NUMBER TEXT, "
                      "ADDRESS TEXT, "
                      "ROLE INTEGER, "      // customer 0 , restaurantmanager 1 , admin 2
                      "IS_ACTIVE INTEGER, " // 1 true , 0 false
                      "CREATED_AT TEXT);";

    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
}

bool UserDAO::Create(const User &user)
{
    // INSERT
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
        user = make_unique<User>(
            sqlite3_column_int64(stmt, 0),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)),
            static_cast<Role>(sqlite3_column_int(stmt, 5)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7)));

        user->SetIs_Active(sqlite3_column_int(stmt, 6) != 0); // user's constructor dosn't take createdAt
    }
    sqlite3_finalize(stmt);
    return user;
}

