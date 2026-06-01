#ifndef CART_HPP
#define CART_HPP

#include <vector>

class Cart
{
public:
    struct CartLine
    {
        long itemId;
        int quantity;
        double unitPrice;
    };

private:
    long customerId;
    long restaurantId;
    std::vector<CartLine> items;

public:
    // constructor
    Cart();
    Cart(long, long);

    // setter
    void SetCustomerId(long);
    void SetRestaurantId(long);

    // getter
    long GetCustomerId() const;
    long GetRestaurantId() const;
    double GetTotalPrice() const;
    bool IsEmpty() const;

    // items operation
    void AddItem(long, int, double);
    bool RemoveItem(long);
    void ChangeItemQuantity(long, int);
    const std::vector<CartLine> &GetItems() const;
    void Clear();
    bool HasItem(long) const;

    void PrintDetails() const;
};
#endif