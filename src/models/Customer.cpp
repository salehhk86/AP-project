#include "Customer.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

// constructor
Customer::Customer() : User(), cart() { SetRole(Role::Customer); }

Customer::Customer(long id, const string &username, const string &pass, const string &phone, const string &address, const string &createdAt)
    : User(id, username, pass, phone, address, Role::Customer, createdAt), cart(id, -1) {}

// access to cart
Cart &Customer::GetCart() { return cart; }
const Cart &Customer::GetCart() const { return cart; }

// access to history
const vector<Order> &Customer::GetOrderHistory() const { return orderHistory; }
void Customer::AddOrderToHistory(const Order &order) { orderHistory.push_back(order); }

// cart operation
void Customer::AddToCart(long id, int quant, double price) { cart.AddItem(id, quant, price); }
bool Customer::RemoveFromCart(long id) { return cart.RemoveItem(id); }
void Customer::ChangeCartQuantity(long id, int quant) { cart.ChangeItemQuantity(id, quant); }
void Customer::ClearCart() { cart.Clear(); }

// cart -> order
Order Customer::Checkout(long orderId, long restaurantId, const string &createdAt)
{
    if (cart.IsEmpty())
    {
        throw std::runtime_error("Cart is empty. Cannot checkout.");
    }

    cart.SetRestaurantId(restaurantId);
    Order order(orderId, GetId(), restaurantId, OrderStatus::Pending, createdAt);

    for (const auto &line : cart.GetItems())
    {
        order.AddLine(line.itemId, line.quantity, line.unitPrice);
    }

    orderHistory.push_back(order);
    cart.Clear();

    return order;
}

void Customer::Print_Details() const
{
    cout << "\n===== Customer =====\n";
    User::Print_Details();
    cout << "Order count: " << orderHistory.size() << '\n';
}