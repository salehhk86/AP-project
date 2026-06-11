#ifndef ADMIN_HPP
#define ADMIN_HPP

#include "User.hpp"
#include "Restaurant.hpp"
#include "Order.hpp"
#include <vector>
#include <memory>
class Admin : public User
{

public:
    // constructor
    Admin();
    Admin(long, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &);

    // admin's operator
    void RegisterRestaurant(std::vector<unique_ptr<Restaurant>> &, unique_ptr<Restaurant> &);
    void ActivateRestaurant(Restaurant &);
    void DeactivateRestaurant(Restaurant &);

    // totality
    int GetActiveUsersCount(const std::vector<std::unique_ptr<User>> &) const;
    int GetActiveRestaurantsCount(const std::vector<std::unique_ptr<Restaurant>> &) const;
    int GetDeliveredOrdersCount(const std::vector<Order> &) const;
    int GetTotalOrdersCount(const std::vector<Order> &) const;
    int GetTotalOrders(const std::vector<Order> &) const;
    double GetTotalSales(const std::vector<Order> &) const;

    void Print_Details() const override;
};
#endif