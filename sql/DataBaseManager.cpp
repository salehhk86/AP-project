#include "DataBaseManager.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;

DataBaseManager::DataBaseManager(const string &dbFile) : db(nullptr)
{
    if (sqlite3_open(dbFile.c_str(), &db) != SQLITE_OK)
    {
        string err = sqlite3_errmsg(db);
        sqlite3_close(db);
        db = nullptr;
        throw runtime_error("Error opening database: " + err);
    }
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

bool DataBaseManager::IsOpen() const { return db != nullptr; }

void DataBaseManager::ExecuteQuery(const string &query)
{
    char *errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        string error = errMsg;
        sqlite3_free(errMsg);
        throw runtime_error("SQL Error: " + error);
    }
}