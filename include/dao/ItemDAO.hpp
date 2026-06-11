#ifndef ITEM_DAO_HPP
#define ITEM_DAO_HPP

#include <vector>
#include <memory>
#include "Item.hpp"
#include <sqlite3.h>

class ItemDAO
{

private:
    sqlite3 *db;

    std::unique_ptr<Item> CreateItemFromRow(sqlite3_stmt*);//for reads methods

public:
    explicit ItemDAO(sqlite3 *);

    //create
    bool CreateTable();
    bool Create(const Item& , long);

    //read
    std::unique_ptr<Item> ReadById(long);
    std::vector<std::unique_ptr<Item>> ReadByRestaurantId(long);

    //update
    bool Update(const Item&);

    //delete
    bool Delete(long);
};

#endif