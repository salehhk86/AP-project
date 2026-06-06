#ifndef RESTAURANT_DAO_HPP
#define RESTAURANT_DAO_HPP

#include "Restaurant.hpp"
#include <sqlite3.h>

class RestaurantDAO
{

private:
    sqlite3 *db;

public:
    explicit RestaurantDAO(sqlite3 *); // using expilicit won't let compiler automatically change a sqlite3* to a restaurantdao.

    // create
    void CreateTable();
    bool Create(const Restaurant &);

    // read
    std::unique_ptr<Restaurant> ReadById(long); // we use unique ptr for handeling the ownership and stop unwanted copying handeling the memory(delete)
    std::vector<std::unique_ptr<Restaurant>> ReadAll();

    // update
    bool Update(const Restaurant &);

    // delete
    bool Delete(long);
};

#endif