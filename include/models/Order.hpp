#ifndef ORDER_HPP
#define ORDER_HPP

#include <vector>
#include <string>

enum class OrderStatus
{
    Pending,     // ثبت شده
    Preparing,   // در حال آماده سازی
    ReadyToSend, // آماده ارسال
    Delivered,   // تحویل داده شده
    Cancelled    // لغو شده
};

class Order
{
public:
    struct OrderLine
    {
        long itemId;
        int quantity;
        double unitPrice;
    };

private:
    long orderId;
    long customerId;
    long restaurantId;
    OrderStatus status;
    std::string createdAt;
    std::vector<OrderLine> lines;

public:
    // constructor
    Order();
    Order(long, long, long, OrderStatus, const std::string &);

    // setter
    void SetOrderId(long);
    void SetCustomerId(long);
    void SetRestaurantId(long);
    void SetStatus(OrderStatus);
    void SetCreatedAt(const std::string &);

    // getter
    long GetOrderId() const;
    long GetCustomerId() const;
    long GetRestaurantId() const;
    OrderStatus GetStatus() const;
    const std::string &GetCreatedAt() const;

    // order lines operation
    double GetTotalPrice() const;
    const std::vector<OrderLine> &GetLines() const;
    void AddLine(long, int, double);
    bool RemoveLine(long);
    void ChangeLineQuantity(long, int);
    void ClearLines();

    const char *OrderStatusToString() const;

    void PrintDetails() const;
};

#endif