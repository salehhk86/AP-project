#include "Food.hpp"


// constructor
Food::Food() // default cons
    : Item(), calories(0.0), isVegan(false), preparationTimeMinutes(0)
{
}

Food::Food(long i, std::string n, std::string d, double p, double t, float c, bool v, int ti)
    : Item(i, n, d, p, t), isVegan(v)
{
    this->SetCalories(c);
    this->SetpreparationTimeMinutes(ti);
}

// Setter
void Food::SetCalories(float c) { calories = (c > 0.0) ? c : 0.0; }
void Food::SetisVegan(bool v) { isVegan = v; }
void Food::SetpreparationTimeMinutes(int t) { preparationTimeMinutes = (t > 0) ? t : 0; }

// Getter
float Food::GetCalories() const { return calories; }
bool Food::GetisVegan() const { return isVegan; }
int Food::GetpreparationTimeMinutes() const { return preparationTimeMinutes; }

ItemType Food::GetType() const { return ItemType::Food; }

void Food::Print() const
{
    std::cout << "[Food] " << GetName() << " | ID: " << GetId() << "\n"
              << "Desc: " << GetDescription() << "\n"
              << "Calories: " << calories << " | Vegan: " << (isVegan ? "Yes" : "No") << "\n"
              << "preparation time: " << preparationTimeMinutes << " min\n"
              << "Price: " << std::fixed << std::setprecision(2) << GetPrice()
              << " | Final: " << FinalPrice() << "\n"
              << "--------------------------\n";
}