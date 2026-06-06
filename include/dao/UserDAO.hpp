#ifndef USER_DAO_HPP
#define USER_DAO_HPP

#include "User.hpp"
#include <memory>
#include <vector>
#include <sqlite3.h>

class UserDAO
{

private:
    sqlite3 *db;

public:
    explicit UserDAO(sqlite3 *);

    //create
    void CreateTable();
    bool Create(const User&);

    //read
    std::unique_ptr<User> ReadById(long id);
    std::vector<std::unique_ptr<User>> ReadAll();

    //update
    bool Update(const User&);

    //delete
    bool Delete(long);
};
#endif