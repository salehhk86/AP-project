#ifndef ADMIN_HPP
#define ADMIN_HPP

#include "User.hpp"
#include "Restaurant.hpp"
#include "Order.hpp"
#include <vector>
class Admin : public User
{

public:
    // constructor
    Admin();
    Admin(long, std::string, std::string, std::string, std::string, std::string);

    // admin's operator
    void RegisterRestaurant(std::vector<Restaurant> &, const Restaurant &);
    void ActivateRestaurant(Restaurant &);
    void DeactivateRestaurant(Restaurant &);

    // totality
    double GetTotalSales(const std::vector<Order> &) const;
    int GetTotalOrders(const std::vector<Order> &) const;

    void Print_Details() const override;
};
#endif