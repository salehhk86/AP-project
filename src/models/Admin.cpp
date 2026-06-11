#include "Admin.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

Admin::Admin() : User() { SetRole(Role::Admin); }

Admin::Admin(long id, const string &username, const string &pass, const string &phone, const string &address, const string &createdAt)
    : User(id, username, pass, phone, address, Role::Admin, createdAt) {}

// admin'S operator
void Admin::RegisterRestaurant(vector<unique_ptr<Restaurant>> &restaurants, unique_ptr<Restaurant> &restaurant)
{
    for (const auto &r : restaurants)
    {
        if (r->GetId() == restaurant->GetId())
        {
            throw std::runtime_error("Restaurant with this ID already exists.");
        }
    }
    restaurants.push_back(move(restaurant));
}
void Admin::ActivateRestaurant(Restaurant &restaurant) { restaurant.SetIsActive(true); }
void Admin::DeactivateRestaurant(Restaurant &restaurant) { restaurant.SetIsActive(false); }

// totality


int Admin::GetActiveUsersCount(const vector<unique_ptr<User>> &users) const
{
    int count = 0;
    for (const auto &user : users)
        if (user->GetIs_Active())
            count++;
    return count;
}
int Admin::GetActiveRestaurantsCount(const vector<unique_ptr<Restaurant>> &restaurants) const
{
    int count = 0;
    for (const auto &restaurant : restaurants)
        if (restaurant->GetIsActive())
            count++;
    return count;
}

int Admin::GetDeliveredOrdersCount(const vector<Order> &orders) const
{
    int count = 0;
    for (const auto &order : orders)
        if (order.GetStatus() == OrderStatus::Delivered)
            count++;
    return count;
}

int Admin::GetTotalOrdersCount(const vector<Order> &orders) const
{
    return static_cast<int>(orders.size());
}

int Admin::GetTotalOrders(const vector<Order> &orders) const
{
    int count = 0;
    for (const auto &order : orders)
        if (order.GetStatus() == OrderStatus::Delivered)
            count++;
    return count;
}

double Admin::GetTotalSales(const vector<Order> &orders) const
{
    double total = 0.0;
    for (const auto &order : orders)
        if (order.GetStatus() == OrderStatus::Delivered)
            total += order.GetTotalPrice();
    return total;
}

void Admin::Print_Details() const
{
    cout << "\n===== Admin =====\n";
    User::Print_Details();
}