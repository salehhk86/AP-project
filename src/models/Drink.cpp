#include "Drink.hpp"

using namespace std;

// constructor
Drink::Drink() // default cons
    : Item(), size(DrinkSize::Medium)
{
}

Drink::Drink(long i, const string &n, const string &d, double p, double t, DrinkSize s)
    : Item(i, n, d, p, t), size(s) {}

// setter
void Drink::SetDrinkSize(DrinkSize s) { size = s; }

// getter
DrinkSize Drink::GetDrinkSize() const { return size; }

ItemType Drink::GetType() const { return ItemType::Drink; }

static const char *ToString(DrinkSize s)
{
    switch (s)
    {
    case DrinkSize::Small:
        return "Small";
    case DrinkSize::Medium:
        return "Medium";
    case DrinkSize::Large:
        return "Large";
    }
    return "Unknown";
}

void Drink::Print() const
{
    cout << fixed << setprecision(2);

    cout << "=== Drink ===\n"
         << "ID: " << GetId() << "\n"
         << "Name: " << GetName() << "\n"
         << "Description: " << GetDescription() << "\n"
         << "Price: " << GetPrice() << "\n"
         << "Tax(%): " << GetTax() << "\n"
         << "Final Price: " << FinalPrice() << "\n"
         << "Status: " << (GetStatus() ? "Available" : "Unavailable") << "\n"
         << "Size: " << ToString(size) << "\n"
         << "=============\n";
}
