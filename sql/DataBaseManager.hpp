#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <sqlite3.h>
#include <string>

class DataBaseManager
{

private:
    sqlite3* db;

public:
    //constructor . opening
    DataBaseManager(const std::string &dbFile);
    //destructor . closing
    ~DataBaseManager();
    //getter . let other classes access to database
    sqlite3* GetDB() const;
    // create . insert . update . delete
    void ExecuteQuery(const std::string &query);
};

#endif