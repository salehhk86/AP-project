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

        // seed default admin
        auto existing = userDAO.ReadByUsername("admin");
        if (!existing)
        {
            time_t now = time(nullptr);
            string createdAt = string(ctime(&now));
            createdAt.pop_back();

            Admin defaultAdmin(0, "saleh", "Admin2580", "09399098787", "System", createdAt);
            defaultAdmin.SetIs_Active(true);

            if (userDAO.Create(defaultAdmin))
                cout << "[INFO] Default admin created -> username: saleh | password: Admin2580\n";
        }

        AppSystem app(userDAO, restaurantDAO, itemDAO, orderDAO);
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