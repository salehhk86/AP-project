#ifndef USER_DAO_HPP
#define USER_DAO_HPP

#include "User.hpp"
#include "Customer.hpp"
#include "Admin.hpp"
#include "RestaurantManager.hpp"

#include <memory>
#include <vector>
#include <sqlite3.h>

class UserDAO
{

private:
    sqlite3 *db;

    std::unique_ptr<User> CreateUserFromRow(sqlite3_stmt *);
    static std::string safeStr(sqlite3_stmt *, int);

public:
    explicit UserDAO(sqlite3 *);

    // create
    void CreateTable();
    bool Create(const User &);

    // read
    std::unique_ptr<User> ReadById(long id);
    std::unique_ptr<User> ReadByUsername(const std::string &);
    std::vector<std::unique_ptr<User>> ReadAll();

    // update
    bool Update(const User &);

    // delete
    bool Delete(long);
};

#endif
