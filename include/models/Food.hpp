#ifndef FOOD_HPP
#define FOOD_HPP

#include "Item.hpp"

class Food : public Item
{

private:
    float calories;
    bool isVegan;
    int preparationTimeMinutes;

public:
    // constructor
    Food();
    Food(long, std::string, std::string, double, double, float, bool, int);

    // setter
    void SetCalories(float);
    void SetisVegan(bool);
    void SetpreparationTimeMinutes(int);

    // getter
    float GetCalories() const;
    bool GetisVegan() const;
    int GetpreparationTimeMinutes() const;

    // virtual's override
    ItemType GetType() const override;
    void Print() const override;
};

#endif