#ifndef RESTAURANT_HPP
#define RESTAURANT_HPP

#include "Item.hpp"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

class Restaurant // has a Item
{

private:
    long id;
    string name;
    string address;
    bool isActive;
    string number;
    string description;

    vector<unique_ptr<Item>> menu; // because of object slicing(food and drink) we should keep the pointer in vector . so i use unique ptr from header memory. that handel the new and delete's of pointer in vector and avoid slicing object or licking memory.

public:
    // constructor
    Restaurant();
    Restaurant(long, string, string, bool, string, string);

    // setter
    void SetId(long);
    void SetName(string &);
    void SetAddress(string &);
    void SetisActive(bool);
    void SetNumber(string &);
    void SetDescription(string &);

    // getter
    long GetId() const;
    const string &GetName() const;
    const string &GetAddress() const;
    bool GetisActive() const;
    const string &GetNumber() const;
    const string &GetDescription() const;

    // menu opration
    void AddMenuItem(unique_ptr<Item>);
    bool RemoveMenuItemById(long);

    Item *FindMenuItemById(long);             // we are not sure we have that item , so we can't surly return reference . we use pointer.
    const Item *FindMenuItemById(long) const; // for const restaurant

    const vector<unique_ptr<Item>> &GetMenu() const;

    void PrintSummary() const;
    void PrintMenu(bool) const;
};
#endif