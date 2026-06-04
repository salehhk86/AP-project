#include "Food.hpp"

using namespace std;

// constructor
Food::Food() // default cons
    : Item(), calories(0.0), isVegan(false), preparationTimeMinutes(0)
{
}

Food::Food(long i, const string &n, const string &d, double p, double t, float c, bool v, int ti)
    : Item(i, n, d, p, t), calories(0.0f), isVegan(v), preparationTimeMinutes(0)
{
    this->SetCalories(c);
    this->SetPreparationTimeMinutes(ti);
}

// Setter
void Food::SetCalories(float c) { calories = (c >= 0.0) ? c : 0.0; }
void Food::SetIsVegan(bool v) { isVegan = v; }
void Food::SetPreparationTimeMinutes(int t) { preparationTimeMinutes = (t > 0) ? t : 0; }

// Getter
float Food::GetCalories() const { return calories; }
bool Food::GetIsVegan() const { return isVegan; }
int Food::GetPreparationTimeMinutes() const { return preparationTimeMinutes; }

ItemType Food::GetType() const { return ItemType::Food; }

void Food::Print() const
{
    cout << "[Food] " << GetName() << " | ID: " << GetId() << "\n"
         << "Desc: " << GetDescription() << "\n"
         << "Calories: " << GetCalories()
         << " | Vegan: " << (GetIsVegan() ? "Yes" : "No") << "\n"
         << "Preparation time: " << GetPreparationTimeMinutes() << " min\n"
         << "Price: " << fixed << setprecision(2) << GetPrice()
         << " | Final: " << FinalPrice() << "\n"
         << "--------------------------\n";
}
