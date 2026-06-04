#include "Item.hpp"
#include <stdexcept>

using namespace std;

// constructor
Item::Item() // default cons
    : id(-1), name(""), description(""), price(0.0), tax(0.0), status(false)
{
}

Item::Item(long i, const std::string &n, const std::string &d, double p, double t)
    : id(i), name(n), description(d), price(0.0), tax(0.0), status(true)
{
    this->SetPrice(p);
    this->SetTax(t);
}

// setter
void Item::SetId(long i) { id = i; }
void Item::SetName(const string &n) { name = n; }
void Item::SetDescription(const string &d) { description = d; }
void Item::SetPrice(double p)
{
    if (p <= 0)
        throw invalid_argument("Price must be greater than zero.");
    price = p;
}
void Item::SetTax(double t)
{
    if (t < 0 || t > 100)
        throw invalid_argument("Tax must be between 0 and 100.");
    tax = t;
}
void Item::SetStatus(bool a) { status = a; }

// getter
long Item::GetId() const { return id; }
const string &Item::GetName() const { return name; }
const string &Item::GetDescription() const { return description; }
double Item::GetPrice() const { return price; }
double Item::GetTax() const { return tax; }
bool Item::GetStatus() const { return status; }

double Item::FinalPrice() const { return price * (tax / 100 + 1); }
