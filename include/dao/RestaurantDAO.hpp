#ifndef RESTAURANT_DAO_HPP
#define RESTAURANT_DAO_HPP

#include "Restaurant.hpp"
#include <sqlite3.h>
#include <vector>
#include <memory>

class RestaurantDAO
{

private:
    sqlite3 *db;

public:
    explicit RestaurantDAO(sqlite3 *); // using explicit won't let compiler automatically convert sqlite3* to RestaurantDAO.

    // create
    void CreateTable();
    bool Create(const Restaurant &);

    // read
    std::unique_ptr<Restaurant> ReadById(long long); // unique_ptr for handling ownership and preventing unwanted copying
    std::vector<std::unique_ptr<Restaurant>> ReadAll();

    // update
    bool Update(const Restaurant &);

    // delete
    bool Delete(long long);
};

#endif
