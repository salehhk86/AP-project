#ifndef RESTAURANT_HPP
#define RESTAURANT_HPP

#include "Item.hpp"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

class Restaurant // has a Item
{

private:
    long id;
    std::string name;
    std::string address;
    bool isActive;
    std::string number;
    std::string description;

    std::vector<std::unique_ptr<Item>> menu; // because of object slicing(food and drink) we should keep the pointer in vector . so i use unique ptr from header memory. that handel the new and delete's of pointer in vector and avoid slicing object or licking memory.

public:
    // constructor
    Restaurant();
    Restaurant(long, const std::string &, const std::string &, bool, const std::string &, const std::string &);

    // setter
    void SetId(long);
    void SetName(const std::string &);
    void SetAddress(const std::string &);
    void SetIsActive(bool);
    void SetNumber(const std::string &);
    void SetDescription(const std::string &);

    // getter
    long GetId() const;
    const std::string &GetName() const;
    const std::string &GetAddress() const;
    bool GetIsActive() const;
    const std::string &GetNumber() const;
    const std::string &GetDescription() const;

    // menu opration
    void AddMenuItem(std::unique_ptr<Item>);
    bool RemoveMenuItemById(long);

    Item *FindMenuItemById(long);             // we are not sure we have that item , so we can't surly return reference . we use pointer.
    const Item *FindMenuItemById(long) const; // for const restaurant

    const std::vector<std::unique_ptr<Item>> &GetMenu() const;

    void PrintSummary() const;
    void PrintMenu(bool = true) const;
};
#endif