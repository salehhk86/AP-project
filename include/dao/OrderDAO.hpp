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
    std::vector<Order::OrderLine> GetLines(long long orderId);

    // helper for enums
    static const char *StatusToString(OrderStatus status);
    static OrderStatus StringToStatus(const std::string &status);

public:
    explicit OrderDAO(sqlite3 *database);

    // create
    void CreateTables();
    bool Create(const Order &order);

    // read
    std::unique_ptr<Order> ReadById(long long orderId);
    std::vector<std::unique_ptr<Order>> ReadByCustomer(long long customerId);

    // update
    bool UpdateStatus(long long orderId, OrderStatus newStatus);

    // delete
    bool Delete(long long orderId);
};

#endif
