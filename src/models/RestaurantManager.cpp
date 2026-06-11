#include "RestaurantManager.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

// private
void RestaurantManager::EnsureOwnership(const Restaurant &restaurant) const
{
    if (restaurant.GetId() != restaurantId)
        throw runtime_error("Manager cannot modify another restaurant.");
}

// constructor
RestaurantManager::RestaurantManager() : User(), restaurantId(-1) { SetRole(Role::RestaurantManager); }

RestaurantManager::RestaurantManager(long id, const string &username, const string &pass, const string &phone, const string &address, const string &createdAt, long restaurantid)
    : User(id, username, pass, phone, address, Role::RestaurantManager, createdAt), restaurantId(restaurantid) {}

// setter
void RestaurantManager::SetRestaurantId(long id) { restaurantId = id; }

// getter
long RestaurantManager::GetRestaurantId() const { return restaurantId; }

// restaurant managing
void RestaurantManager::AddMenuItem(Restaurant &restaurant, unique_ptr<Item> item)
{
    EnsureOwnership(restaurant);

    if (!item)
        throw invalid_argument("Item pointer is null.");

    restaurant.AddMenuItem(move(item));
}

bool RestaurantManager::RemoveMenuItem(Restaurant &restaurant, long itemId)
{
    EnsureOwnership(restaurant);

    return restaurant.RemoveMenuItemById(itemId);
}

void RestaurantManager::UpdateMenuItemPrice(Restaurant &restaurant, long itemId, double newPrice)
{
    EnsureOwnership(restaurant);

    Item *item = restaurant.FindMenuItemById(itemId);
    if (!item)
        throw runtime_error("Menu item not found.");

    item->SetPrice(newPrice);
}

void RestaurantManager::UpdateMenuItemDescription(Restaurant &restaurant, long itemId, const string &newDesc)
{
    EnsureOwnership(restaurant);

    Item *item = restaurant.FindMenuItemById(itemId);
    if (!item)
        throw runtime_error("Menu item not found.");
    item->SetDescription(newDesc);
}

void RestaurantManager::UpdateMenuItemAvailability(Restaurant &restaurant, long itemId, bool available)
{
    EnsureOwnership(restaurant);

    Item *item = restaurant.FindMenuItemById(itemId);
    if (!item)
        throw runtime_error("Menu item not found.");
    item->SetStatus(available);
}

void RestaurantManager::UpdateRestaurantName(Restaurant &restaurant, const string &name)
{
    EnsureOwnership(restaurant);
    restaurant.SetName(name);
}
void RestaurantManager::UpdateRestaurantAddress(Restaurant &restaurant, const string &address)
{
    EnsureOwnership(restaurant);
    restaurant.SetAddress(address);
}
void RestaurantManager::UpdateRestaurantDescription(Restaurant &restaurant, const string &desc)
{
    EnsureOwnership(restaurant);
    restaurant.SetDescription(desc);
}

// orders
void RestaurantManager::UpdateOrderStatus(Order &order, OrderStatus newStatus)
{
    if (order.GetRestaurantId() != restaurantId)
        throw runtime_error("Manager cannot update orders of another restaurant.");

    if (newStatus == OrderStatus::Cancelled)
        throw runtime_error("Only customer can cancel an order.");

    order.SetStatus(newStatus);
}

void RestaurantManager::Print_Details() const
{
    cout << "\n===== RestaurantManager =====\n";
    User::Print_Details();
    cout << "Restaurant id: " << restaurantId << '\n';
}