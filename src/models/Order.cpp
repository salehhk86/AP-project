#include "Order.hpp"

using namespace std;

// constructor
Order::Order() : orderId(-1), customerId(-1), restaurantId(-1),
                 status(OrderStatus::Pending), createdAt("") {}

Order::Order(long o, long c, long r, OrderStatus s, string ca)
    : orderId(o), customerId(c), restaurantId(r), status(s), createdAt(ca) {}

// setter
void Order::SetOrderId(long o) { orderId = o; }
void Order::SetCustomerId(long c) { customerId = c; }
void Order::SetRestaurantId(long r) { restaurantId = r; }
void Order::SetStatus(OrderStatus s) { status = s; }
void Order::SetCreatedAt(string ca) { createdAt = ca; }

// getter
long Order::GetOrderId() const { return orderId; }
long Order::GetCustomerId() const { return customerId; }
long Order::GetRestaurantId() const { return restaurantId; }
OrderStatus Order::GetStatus() const { return status; }
string Order::GetCreatedAt() const { return createdAt; }

// order lines operation
double Order::GetTotalPrice() const
{
    double totalPrice = 0.0;
    for (const auto &a : lines)
    {
        totalPrice += (a.quantity * a.unitPrice);
    }
    return totalPrice;
}

const vector<Order::OrderLine> &Order::GetLines() const { return lines; }

void Order::AddLine(long id, int quant, double price)
{
    if (quant <= 0 || price < 0)
        return;

    for (auto &a : lines)
    {
        if (id == a.itemId)
        {
            a.quantity += quant;
            return;
        }
    }

    lines.push_back({id, quant, price});
}

bool Order::RemoveLine(long id)
{
    for (auto l = lines.begin(); l != lines.end(); l++)
    {
        if (id == l->itemId)
        {
            lines.erase(l);
            return true;
        }
    }
    return false;
}

void Order::ChangeLineQuantity(long id, int newQuant)
{
    if (newQuant < 0)
        return;

    for (auto l = lines.begin(); l != lines.end(); l++)
    {
        if (id == l->itemId)
        {
            if (newQuant == 0)
            {
                lines.erase(l);
            }
            else
            {
                l->quantity = newQuant;
            }
            return;
        }
    }
}

void Order::ClearLines() { lines.clear(); }

string Order::OrderStatusToString() const
{
    switch (status)
    {
    case OrderStatus::Pending:
        return "Pending";

    case OrderStatus::Preparing:
        return "Preparing";

    case OrderStatus::ReadyToSend:
        return "ReadyToSend";

    case OrderStatus::Delivered:
        return "Delivered";

    case OrderStatus::Cancelled:
        return "Cancelled";

    default:
        return "Unknown";
    }
}

void Order::PrintDetails() const
{
    cout << "\n===== Order Details ====="
         << "\nOrder ID: " << orderId
         << "\nCustomer ID: " << customerId
         << "\nRestaurant ID: " << restaurantId
         << "\nCreated At: " << createdAt
         << "\nStatus: " << this->OrderStatusToString()
         << "\nItems:";
    for (const auto &l : lines)
    {
        cout << "\nItem ID: " << l.itemId
             << " | Quantity: " << l.quantity
             << " | Unit Price: " << l.unitPrice
             << " | Line Total: " << (l.quantity * l.unitPrice);
    }

    cout << "\nTotal Price: " << GetTotalPrice() << endl;
}