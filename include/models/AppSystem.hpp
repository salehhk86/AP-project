#ifndef APPSYSTEM_HPP
#define APPSYSTEM_HPP

#include <memory>
#include <string>
#include "User.hpp"
#include "Admin.hpp"
#include "Customer.hpp"
#include "RestaurantManager.hpp"
#include "Restaurant.hpp"
#include "UserDAO.hpp"
#include "RestaurantDAO.hpp"
#include "ItemDAO.hpp"
#include "OrderDAO.hpp"

class AppSystem
{
private:
    UserDAO &userDAO;
    RestaurantDAO &restaurantDAO;
    ItemDAO &itemDAO;
    OrderDAO &orderDAO;

    std::unique_ptr<User> currentUserOwner; // ownership
    User *currentUser;                     

public:
    AppSystem(UserDAO &, RestaurantDAO &, ItemDAO &, OrderDAO &);

    bool SignUp(const std::string &, const std::string &,
                const std::string &, const std::string &,
                Role, long restaurantId = -1);

    User *SignIn(const std::string &, const std::string &);
    void SignOut();
    User *GetCurrentUser() const;

    UserDAO &GetUserDAO() const { return userDAO; }
    RestaurantDAO &GetRestaurantDAO() const { return restaurantDAO; }
    ItemDAO &GetItemDAO() const { return itemDAO; }
    OrderDAO &GetOrderDAO() const { return orderDAO; }
};

#endif