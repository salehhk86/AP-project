#ifndef ORDER_DAO_HPP
#define ORDER_DAO_HPP

#include "Order.hpp"
#include <sqlite3.h>
#include <vector>
#include <memory>
#include <string>

class OrderDAO
{
private:
    sqlite3 *db;

    // helper to fetch lines for a specific order
    std::vector<Order::OrderLine> GetLines(long long);

    // helper for enums
    static const char *StatusToString(OrderStatus);
    static OrderStatus StringToStatus(const std::string &);

public:
    explicit OrderDAO(sqlite3 *);

    // create
    void CreateTables();
    bool Create(const Order &);

    // read
    std::unique_ptr<Order> ReadById(long long);
    std::vector<std::unique_ptr<Order>> ReadByCustomer(long long);

    // update
    bool UpdateStatus(long long, OrderStatus);

    // delete
    bool Delete(long long);
};

#endif
