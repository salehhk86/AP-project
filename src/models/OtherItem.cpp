#include "OtherItem.hpp"

using namespace std;

// constructor
OtherItem::OtherItem() // default cons
    : Item()
{
}

OtherItem::OtherItem(long i, const std::string &n, const std::string &d, double p, double t)
    : Item(i, n, d, p, t) {}

ItemType OtherItem::GetType() const { return ItemType::Other; }

void OtherItem::Print() const
{
    cout << "=== Other Items ===" << GetName() 
         << " | ID: " << GetId() << "\n"
         << "Desc: " << GetDescription() << "\n"
         << "Price: " << fixed << setprecision(2) << GetPrice()
         << " | Final: " << FinalPrice() << "\n"
         << "--------------------------\n";
}
