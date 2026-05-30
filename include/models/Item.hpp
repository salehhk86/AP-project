#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <iostream>
#include <iomanip>

enum class ItemType
{
    Food,
    Drink,
    Other
};

class Item
{

protected:
    long id;
    std::string name;
    std::string description;
    double price;
    double tax;
    bool status; // available or not

public:
    // constructor
    Item(); // default
    Item(long, std::string, std::string, double, double);

    // distructor
    virtual ~Item();

    // setter
    void SetId(long);
    void SetName(std::string);
    void SetDescription(std::string);
    void SetPrice(double);
    void SetTax(double);
    void SetStatus(bool);

    // getter
    long GetId() const;
    std::string GetName() const;
    std::string GetDescription() const;
    double GetPrice() const;
    double GetTax() const;
    bool GetStatus() const;

    double FinalPrice() const;

    virtual ItemType GetType() const = 0;
    virtual void Print() const = 0;
};

#endif