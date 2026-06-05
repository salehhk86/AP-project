#include "DataBaseManager.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;

DataBaseManager::DataBaseManager(const string &dbFile) : db(nullptr)
{
    if (sqlite3_open(dbFile.c_str(), &db) != SQLITE_OK)
    {
        cerr << "Erorr open Database : " << sqlite3_errmsg(db) << endl;
    }
    else
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
    char *massageError = nullptr;
    int result = sqlite3_exec(db, query.c_str(), nullptr, 0, &massageError);
    if (result != SQLITE_OK)
    {
        string error = massageError;
        sqlite3_free(massageError);
        throw runtime_error("SQL ERORR : " + error);
    }
    else
        cout << "Query done Seccussfully!\n";
}