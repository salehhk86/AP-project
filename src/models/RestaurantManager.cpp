#include "RestaurantManager.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

// constructor
RestaurantManager::RestaurantManager() : User(), restaurantId(-1) { SetRole(Role::RestaurantManager); }

RestaurantManager::RestaurantManager(long id, string username, string pass, string phone, string address, string createdAt, long Rid)
    : User(id, username, pass, phone, address, Role::RestaurantManager, createdAt), restaurantId(Rid) {}

// setter
void RestaurantManager::SetRestaurantId(long id) { restaurantId = id; }

// getter
long RestaurantManager::GetRestaurantId() const { return restaurantId; }

// restaurant managing
void RestaurantManager::AddMenuItem(Restaurant &restaurant, unique_ptr<Item> item)
{
    if (restaurant.GetId() != restaurantId)
        throw runtime_error("Manager cannot modify another restaurant.");
    restaurant.AddMenuItem(move(item));
}

bool RestaurantManager::RemoveMenuItem(Restaurant &restaurant, long itemId)
{
    if (restaurant.GetId() != restaurantId)
        throw runtime_error("Manager cannot modify another restaurant.");
    return restaurant.RemoveMenuItemById(itemId);
}

void RestaurantManager::UpdateMenuItemPrice(Restaurant &restaurant, long itemId, double newPrice)
{
    if (restaurant.GetId() != restaurantId)
        throw runtime_error("Manager cannot modify another restaurant.");

    Item *item = restaurant.FindMenuItemById(itemId);
    if (!item)
        throw std::runtime_error("Menu item not found.");
    item->SetPrice(newPrice);
}

void RestaurantManager::UpdateMenuItemDescription(Restaurant &restaurant, long itemId, const string &newDesc)
{
    if (restaurant.GetId() != restaurantId)
        throw runtime_error("Manager cannot modify another restaurant.");

    Item *item = restaurant.FindMenuItemById(itemId);
    if (!item)
        throw std::runtime_error("Menu item not found.");
    item->SetDescription(newDesc);
}

void RestaurantManager::UpdateMenuItemAvailability(Restaurant &restaurant, long itemId, bool available)
{
    if (restaurant.GetId() != restaurantId)
        throw runtime_error("Manager cannot modify another restaurant.");

    Item *item = restaurant.FindMenuItemById(itemId);
    if (!item)
        throw std::runtime_error("Menu item not found.");
    item->SetStatus(available);
}

// orders
void RestaurantManager::UpdateOrderStatus(Order &order, OrderStatus newStatus)
{
    if (order.GetRestaurantId() != restaurantId)
        throw runtime_error("Manager cannot update orders of another restaurant.");
    order.SetStatus(newStatus);
}

void RestaurantManager::Print_Details() const
{
    cout << "RestaurantManager\n";
    User::Print_Details();
    cout << "Restaurant id: " << restaurantId << '\n';
}
