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
    explicit DataBaseManager(const std::string &);
    //destructor . closing
    ~DataBaseManager();

    //delete copy constructor and assignment operator to prevent copying
    DataBaseManager(const DataBaseManager &) = delete;
    DataBaseManager &operator=(const DataBaseManager &) = delete;


    //getter . let other classes access to database
    sqlite3* GetDB() const;
    // create . insert . update . delete
    void ExecuteQuery(const std::string &);

    bool IsOpen() const;
};

#endif