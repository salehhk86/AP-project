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
    Item(long, const std::string &, const std::string &, double, double);

    // destructor
    virtual ~Item() = default;

    // setter
    void SetId(long);
    void SetName(const std::string &);
    void SetDescription(const std::string &);
    void SetPrice(double);
    void SetTax(double);
    void SetStatus(bool);

    // getter
    long GetId() const;
    const std::string &GetName() const;
    const std::string &GetDescription() const;
    double GetPrice() const;
    double GetTax() const;
    bool GetStatus() const;

    double FinalPrice() const;

    virtual ItemType GetType() const = 0;
    virtual void Print() const = 0;
};

#endif