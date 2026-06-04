#ifndef DRINK_HPP
#define DRINK_HPP

#include "Item.hpp"

enum class DrinkSize
{
    Small,
    Medium,
    Large
};

class Drink : public Item
{

private:
    DrinkSize size;

public:
    // constructor
    Drink();
    Drink(long, const std::string &, const std::string &, double, double, DrinkSize);

    // Setter
    void SetDrinkSize(DrinkSize);

    // Getter
    DrinkSize GetDrinkSize() const;

    // virtual's override
    ItemType GetType() const override;
    void Print() const override;
};

#endif