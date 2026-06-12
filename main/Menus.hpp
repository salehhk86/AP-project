#ifndef MENUS_HPP
#define MENUS_HPP

#include "AppSystem.hpp"
#include "Customer.hpp"
#include "RestaurantManager.hpp"
#include "Admin.hpp"

class Menus
{
private:
    AppSystem &app;

    // helpers
    int GetIntInput(const std::string &);
    std::string GetStringInput(const std::string &);
    void ClearScreen();
    void PauseScreen();

    //main menu
    void LoginMenu();
    void RegisterMenu();

    //user's menu
    void CustomerMenu(Customer *);
    void ManagerMenu(RestaurantManager *);
    void AdminMenu(Admin *);

    //other option
    void BrowseRestaurants(Customer *);
    void ViewCart(Customer *);
    void OrderHistory(Customer *);

    //manager's menu
    void ManageMenu(RestaurantManager *);
    void ViewOrders(RestaurantManager *);

    //admin's menu
    void RegisterRestaurant();
    void ToggleRestaurant();
    void ViewReports();
    void AssignManager();
    void ListAllUsers();

public:
    explicit Menus(AppSystem &);
    void MainMenu();
};

#endif