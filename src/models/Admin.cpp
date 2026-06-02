#include "Admin.hpp"
#include <iostream>

using namespace std;

Admin::Admin() : User() { SetRole(Role::Admin); }

Admin::Admin(long id, string username, string pass, string phone, string address, string createdAt)
    : User(id, username, pass, phone, address, Role::Admin, createdAt) {}

// admin'S operator
void Admin::RegisterRestaurant(vector<Restaurant> &restaurants, const Restaurant &restaurant) { restaurants.push_back(restaurant); }
void Admin::ActivateRestaurant(Restaurant &restaurant) { restaurant.SetisActive(true); }
void Admin::DeactivateRestaurant(Restaurant &restaurant) { restaurant.SetisActive(false); }

// totality
double Admin::GetTotalSales(const vector<Order> &orders) const
{
    double total = 0.0;

    for (const auto &order : orders)
    {
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