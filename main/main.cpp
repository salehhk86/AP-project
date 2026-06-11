#include <iostream>
#include "DataBaseManager.hpp"
#include "UserDAO.hpp"
#include "RestaurantDAO.hpp"
#include "ItemDAO.hpp"
#include "OrderDAO.hpp"
#include "AppSystem.hpp"
#include "Menus.hpp"

using namespace std;

int main()
{
    try
    {
        DataBaseManager dbManager("food_app.db");
        sqlite3 *db = dbManager.GetDB();

        UserDAO userDAO(db);
        RestaurantDAO restaurantDAO(db);
        ItemDAO itemDAO(db);
        OrderDAO orderDAO(db);

        userDAO.CreateTable();
        restaurantDAO.CreateTable();
        itemDAO.CreateTable();
        orderDAO.CreateTables();

        AppSystem app(userDAO, restaurantDAO, itemDAO, orderDAO);

        //main loop
        Menus menus(app);
        menus.MainMenu();
    }
    catch (const exception &e)
    {
        cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}