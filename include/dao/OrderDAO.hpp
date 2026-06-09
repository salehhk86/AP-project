#ifndef ORDER_DAO_HPP
#define ORDER_DAO_HPP

#include "Order.hpp"
#include <sqlite3.h>
#include <vector>
#include <memory>
#include <string>

using namespace std;

class OrderDAO
{
private:
    sqlite3 *db;

    vector<Order::OrderLine> GetLines(long orderId);

    // helper 
    static const char *StatusToString(OrderStatus status);
    static OrderStatus StringToStatus(const string &status);

public:
    explicit OrderDAO(sqlite3 *database);

    //create
    void CreateTables();
    bool Create(const Order &order);

    //read
    unique_ptr<Order> ReadById(long orderId);
    vector<unique_ptr<Order>> ReadByCustomer(long customerId);

    //update
    bool UpdateStatus(long orderId, OrderStatus newStatus);

    //delete
    bool Delete(long orderId);
};

#endif
