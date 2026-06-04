#include "Admin.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

Admin::Admin() : User() { SetRole(Role::Admin); }

Admin::Admin(long id, const string &username, const string &pass, const string &phone, const string &address, const string &createdAt)
    : User(id, username, pass, phone, address, Role::Admin, createdAt) {}

// admin'S operator
void Admin::RegisterRestaurant(vector<Restaurant> &restaurants, const Restaurant &restaurant)
{
    for (const auto &r : restaurants)
    {
        if (r.GetId() == restaurant.GetId())
        {
            throw std::runtime_error("Restaurant with this ID already exists.");
        }
    }
    restaurants.push_back(restaurant);
}
void Admin::ActivateRestaurant(Restaurant &restaurant) { restaurant.SetIsActive(true); }
void Admin::DeactivateRestaurant(Restaurant &restaurant) { restaurant.SetIsActive(false); }

// totality
double Admin::GetTotalSales(const vector<Order> &orders) const
{
    double total = 0.0;

    for (const auto &order : orders)
    {
        if (order.GetStatus() == OrderStatus::Delivered)
            total += order.GetTotalPrice();
    }
    return total;
}

int Admin::GetTotalOrders(const vector<Order> &orders) const
{
    return static_cast<int>(orders.size());
}

void Admin::Print_Details() const
{
    cout << "Admin\n";
    User::Print_Details();
}