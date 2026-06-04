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
    Food(long, const std::string &, const std::string &, double, double, float, bool, int);

    // setter
    void SetCalories(float);
    void SetIsVegan(bool);
    void SetPreparationTimeMinutes(int);

    // getter
    float GetCalories() const;
    bool GetIsVegan() const;
    int GetPreparationTimeMinutes() const;

    // virtual's override
    ItemType GetType() const override;
    void Print() const override;
};

#endif