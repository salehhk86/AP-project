#include <Restaurant.hpp>

// constructor
Restaurant::Restaurant() : id(-1), name(""), address(""),
                           isActive(false), number(""), description("") {}

Restaurant::Restaurant(long i, string nam, string a, bool ac, string num, string desc)
    : id(i), name(nam), address(a), isActive(ac), number(num), description(desc) {}

// setter
void Restaurant::SetId(long i) { id = i; }
void Restaurant::SetName(string &nam) { name = nam; }
void Restaurant::SetAddress(string &a) { address = a; }
void Restaurant::SetisActive(bool ac) { isActive = ac; }
void Restaurant::SetNumber(string &num) { number = num; }
void Restaurant::SetDescription(string &desc) { description = desc; }

// getter
long Restaurant::GetId() const { return id; }
const string &Restaurant::GetName() const { return name; }
const string &Restaurant::GetAddress() const { return address; }
bool Restaurant::GetisActive() const { return isActive; }
const string &Restaurant::GetNumber() const { return number; }
const string &Restaurant::GetDescription() const { return description; }

// menu operation
void Restaurant::AddMenuItem(unique_ptr<Item> item)
{
    if (!item)
        return menu.push_back(move(item)); // it is a unique ptr so we can't take a copy . we move the orginal in vector (move)! // now item is nullptr
}

bool Restaurant::RemoveMenuItemById(long id)
{
    for (auto item = menu.begin(); item != menu.end(); item++) // item is a uniqe ptr in vecrot and menu.begin give us the address of first ptr of vector
    {
        if (*item && (*item)->GetId() == id)
        {
            menu.erase(item);
            return true;
        }
    }
    return false;
    // item is a iterator
}

Item *Restaurant::FindMenuItemById(long id)
{
    for (const auto &item : menu)
    {
        if (item && item->GetId() == id)
            return item.get(); // an operator in unique ptr . return the raw ptr.(normal ptr we've used)
    }
    return nullptr;
}

const Item *Restaurant::FindMenuItemById(long id) const
{
    for (const auto &item : menu)
    {
        if (item && item->GetId() == id)
            return item.get();
    }
    return nullptr;
    // it's the same last methos . but for const restaurant.
}

const vector<unique_ptr<Item>> &Restaurant::GetMenu() const
{
    return menu;
}

void Restaurant::PrintSummary() const
{
    cout << "Restaurant #" << id << " | " << name << "\n"
         << "Status: " << (isActive ? "Active" : "Inactive") << "\n"
         << "Address: " << address << "\n"
         << "Phone: " << number << "\n"
         << "Description: " << description << "\n";
}

void Restaurant::PrintMenu(bool onlyAvailable = true) const
{
    cout << "--- Menu of " << name << " ---\n";

    if (menu.empty())
    {
        cout << "(empty)\n";
        return;
    }

    for (const auto &item : menu)
    {
        if (!item)
            continue;

        if (onlyAvailable && !item->GetStatus())
            continue;

        item->Print();
    }
}