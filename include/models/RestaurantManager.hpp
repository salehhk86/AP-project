#ifndef RESTAURANTMANAGER_HPP
#define RESTAURANTMANAGER_HPP

#include "User.hpp"
#include "Restaurant.hpp"
#include "Order.hpp"
#include "Item.hpp"
#include <string>
#include <memory>
class RestaurantManager : public User
{

private:
    long restaurantId;

public:
    // constructor
    RestaurantManager();
    RestaurantManager(long, std::string, std::string, std::string, std::string, std::string, long);

    // setter
    void SetRestaurantId(long);

    // getter
    long GetRestaurantId() const;

    // restaurant managing
    void AddMenuItem(Restaurant &, std::unique_ptr<Item>);
    bool RemoveMenuItem(Restaurant &, long);
    void UpdateMenuItemPrice(Restaurant &, long, double);
    void UpdateMenuItemDescription(Restaurant &, long, const std::string &);
    void UpdateMenuItemAvailability(Restaurant &, long, bool);

    // orders
    void UpdateOrderStatus(Order &, OrderStatus);

    void Print_Details() const override;
};
#endif