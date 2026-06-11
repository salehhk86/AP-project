#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include "User.hpp"
#include "Cart.hpp"
#include "Order.hpp"
#include <vector>
#include <string>

class Customer : public User
{

private:
    Cart cart;
    std::vector<Order> orderHistory;

public:
    // constructor
    Customer();
    Customer(long, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &);

    // access to cart
    Cart &GetCart();
    const Cart &GetCart() const;

    // access to history
    const std::vector<Order> &GetOrderHistory() const;

    // cart operation
    void AddToCart(long, int, double);
    bool RemoveFromCart(long);
    void ChangeCartQuantity(long, int);
    void ClearCart();

    // cart -> order
    Order Checkout(long, long, const std::string &);

    void Print_Details() const override;
};
#endif