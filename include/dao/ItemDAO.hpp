#ifndef ITEM_DAO_HPP
#define ITEM_DAO_HPP

#include "Item.hpp"
#include "Food.hpp"
#include "Drink.hpp"
#include "OtherItem.hpp"
#include <vector>
#include <memory>
#include <sqlite3.h>

class ItemDAO
{

private:
    sqlite3 *db;

    std::unique_ptr<Item> CreateItemFromRow(sqlite3_stmt* stmt);//for reads methods

public:
    explicit ItemDAO(sqlite3 *);

    //create
    void CreateTable();
    bool Create(const Item&);

    //read
    std::unique_ptr<Item> ReadById(long);
    std::vector<std::unique_ptr<Item>> ReadAll();

    //update
    bool Update(const Item&);

    //delete
    bool Delete(long);
};

#endif