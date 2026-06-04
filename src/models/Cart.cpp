#include "Cart.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

// constructor
Cart::Cart() : customerId(-1), restaurantId(-1) {}

Cart::Cart(long c, long r) : customerId(c), restaurantId(r) {}

// Setter
void Cart::SetCustomerId(long c) { customerId = c; }
void Cart::SetRestaurantId(long r) { restaurantId = r; }

// getter
long Cart::GetCustomerId() const { return customerId; }
long Cart::GetRestaurantId() const { return restaurantId; }
double Cart::GetTotalPrice() const
{
    double total = 0.0;
    for (const auto &a : items)
    {
        total += a.quantity * a.unitPrice;
    }
    return total;
}

bool Cart::IsEmpty() const { return items.empty(); }

void Cart::AddItem(long id, int quant, double price)
{
    if (quant <= 0 || price < 0)
        return;

    for (auto &a : items)
    {
        if (a.itemId == id)
        {
            a.quantity += quant;
            return;
        }
    }
    items.push_back({id, quant, price});
}

bool Cart::RemoveItem(long id)
{
    for (auto it = items.begin(); it != items.end(); it++)
    {
        if (it->itemId == id)
        {
            items.erase(it);
            return true;
        }
    }
    return false;
}

void Cart::ChangeItemQuantity(long id, int quant)
{
    if (quant < 0)
        throw std::invalid_argument("Quantity cannot be negative.");

    for (auto it = items.begin(); it != items.end(); it++)
    {
        if (it->itemId == id)
        {
            if (quant == 0)
            {
                items.erase(it);
            }
            else
            {
                it->quantity = quant;
            }
            return;
        }
    }
}

const vector<Cart::CartLine> &Cart::GetItems() const { return items; }

void Cart::Clear() { items.clear(); }

bool Cart::HasItem(long id) const
{
    for (const auto &a : items)
    {
        if (a.itemId == id)
        {
            return true;
        }
    }

    return false;
}

void Cart::PrintDetails() const
{
    cout << "\n--- Cart Details ---\n"
         << "Customer ID: " << customerId << "\n"
         << "Restaurant ID: " << restaurantId << "\n";

    if (IsEmpty())
    {
        cout << "Cart is empty.\n";
        return;
    }

    cout << left << setw(10) << "Item ID"
         << setw(10) << "Qty"
         << setw(10) << "Price" << "\n";

    for (const auto &item : items)
    {
        cout << left << setw(10) << item.itemId
             << setw(10) << item.quantity
             << setw(10) << item.unitPrice << "\n";
    }

    cout << "Total Price: $" << GetTotalPrice()
         << "\n------------------------------------------\n";
}