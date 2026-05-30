#include "Item.hpp"

// constructor
Item::Item() // default cons
    : id(-1), name(""), description(""), price(0.0), tax(0.0), status(false)
{
}

Item::Item(long i, std::string n, std::string d, double p, double t)
    : id(i), name(n), description(d), status(true)
{
    this->SetPrice(p);
    this->SetTax(t);
}

Item::~Item() = default;

// setter
void Item::SetId(long i) { id = i; }
void Item::SetName(std::string n) { name = n; }
void Item::SetDescription(std::string d) { description = d; }
void Item::SetPrice(double p) { price = (p > 0) ? p : 0.0; }
void Item::SetTax(double t) { tax = (t >= 0 && t < 100) ? t : 0.0; }
void Item::SetStatus(bool a) { status = a; }

// getter
long Item::GetId() const { return id; }
std::string Item::GetName() const { return name; }
std::string Item::GetDescription() const { return description; }
double Item::GetPrice() const { return price; }
double Item::GetTax() const { return tax; }
bool Item::GetStatus() const { return status; }

double Item::FinalPrice() const { return price * (tax / 100 + 1); }
