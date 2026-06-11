#include "DataBaseManager.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;

DataBaseManager::DataBaseManager(const string &dbFile) : db(nullptr)
{
    if (sqlite3_open(dbFile.c_str(), &db) != SQLITE_OK)
        throw runtime_error("Error opening database: " + string(sqlite3_errmsg(db)));
    cout << "Opened Database Successfully!\n";
}

DataBaseManager::~DataBaseManager()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr;
        cout << "Database Closed Successfully!\n";
    }
}

sqlite3 *DataBaseManager::GetDB() const { return db; }

void DataBaseManager::ExecuteQuery(const string &query)
{
    char *messageError = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &messageError) != SQLITE_OK)
    {
        string error = messageError;
        sqlite3_free(messageError);
        throw runtime_error("SQL Error: " + error);
    }
}