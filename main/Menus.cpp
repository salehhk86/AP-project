#include "Menus.hpp"
#include <iostream>
#include <limits>
#include "Item.hpp"
#include "Drink.hpp"
#include "Food.hpp"

using namespace std;

Menus::Menus(AppSystem &a) : app(a) {}

// helpers
int Menus::GetIntInput(const string &prompt)
{
    int val;
    while (true)
    {
        cout << prompt;
        if (cin >> val)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
    }
}

string Menus::GetStringInput(const string &prompt)
{
    string val;
    cout << prompt;
    getline(cin, val);
    return val;
}

void Menus::ClearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Menus::PauseScreen()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// منوی اصلی
void Menus::MainMenu()
{
    while (true)
    {
        ClearScreen();
        cout << "============================\n";
        cout << "   Online Food Order System \n";
        cout << "============================\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "0. Exit\n";
        cout << "============================\n";

        int choice = GetIntInput("Enter choice: ");

        switch (choice)
        {
        case 1:
            LoginMenu();
            break;
        case 2:
            RegisterMenu();
            break;
        case 0:
            cout << "Goodbye!\n";
            return;
        default:
            cout << "Invalid choice.\n";
        }
    }
}

void Menus::LoginMenu()
{
    ClearScreen();
    cout << "===== Login =====\n";
    string username = GetStringInput("Username: ");
    string password = GetStringInput("Password: ");

    User *user = app.SignIn(username, password);
    if (!user)
    {
        PauseScreen();
        return;
    }

    if (user->GetRole() == Role::Customer)
        CustomerMenu(dynamic_cast<Customer *>(user));
    else if (user->GetRole() == Role::RestaurantManager)
        ManagerMenu(dynamic_cast<RestaurantManager *>(user));
    else if (user->GetRole() == Role::Admin)
        AdminMenu(dynamic_cast<Admin *>(user));

    app.SignOut();
}

void Menus::RegisterMenu()
{
    ClearScreen();
    cout << "===== Register =====\n";
    string username = GetStringInput("Username: ");
    string password = GetStringInput("Password: ");
    string phone = GetStringInput("Phone: ");
    string address = GetStringInput("Address: ");

    app.SignUp(username, password, phone, address, Role::Customer);
    PauseScreen();
}

void Menus::CustomerMenu(Customer *customer)
{
    while (true)
    {
        ClearScreen();
        cout << "===== Customer Menu =====\n";
        cout << "Welcome, " << customer->GetUserName() << "\n";
        cout << "1. Browse Restaurants\n";
        cout << "2. View Cart\n";
        cout << "3. Order History\n";
        cout << "0. Logout\n";
        cout << "=========================\n";

        int choice = GetIntInput("Enter choice: ");
        switch (choice)
        {
        case 1:
            BrowseRestaurants(customer);
            break;
        case 2:
            ViewCart(customer);
            break;
        case 3:
            OrderHistory(customer);
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice.\n";
            PauseScreen();
        }
    }
}

void Menus::BrowseRestaurants(Customer *customer)
{
    while (true)
    {
        ClearScreen();
        auto restaurants = app.GetRestaurantDAO().ReadAllActive();

        if (restaurants.empty())
        {
            cout << "No active restaurants available.\n";
            PauseScreen();
            return;
        }

        cout << "===== Restaurants =====\n";
        for (int i = 0; i < (int)restaurants.size(); i++)
            cout << i + 1 << ". " << restaurants[i]->GetName()
                 << " | " << restaurants[i]->GetAddress() << "\n";
        cout << "0. Back\n";
        cout << "=======================\n";

        int choice = GetIntInput("Select restaurant: ");
        if (choice == 0)
            return;
        if (choice < 1 || choice > (int)restaurants.size())
        {
            cout << "Invalid choice.\n";
            PauseScreen();
            continue;
        }

        Restaurant *selected = restaurants[choice - 1].get();
        customer->GetCart().SetRestaurantId(selected->GetId());

        // loading items from db
        auto items = app.GetItemDAO().ReadByRestaurantId(selected->GetId());
        for (auto &item : items)
            selected->AddMenuItem(move(item));

        // show the menu and choosing
        while (true)
        {
            ClearScreen();
            cout << "===== Menu: " << selected->GetName() << " =====\n";
            const auto &menu = selected->GetMenu();

            if (menu.empty())
            {
                cout << "No items available.\n";
                PauseScreen();
                break;
            }

            for (int i = 0; i < (int)menu.size(); i++)
            {
                if (!menu[i]->GetStatus())
                    continue;
                cout << i + 1 << ". " << menu[i]->GetName()
                     << " | " << menu[i]->FinalPrice() << " Toman"
                     << " | " << menu[i]->GetDescription() << "\n";
            }
            cout << "0. Back\n";
            cout << "================================\n";

            int itemChoice = GetIntInput("Select item to add to cart: ");
            if (itemChoice == 0)
                break;
            if (itemChoice < 1 || itemChoice > (int)menu.size())
            {
                cout << "Invalid choice.\n";
                PauseScreen();
                continue;
            }

            Item *item = menu[itemChoice - 1].get();
            if (!item->GetStatus())
            {
                cout << "Item not available.\n";
                PauseScreen();
                continue;
            }

            int qty = GetIntInput("Quantity: ");
            if (qty <= 0)
            {
                cout << "Invalid quantity.\n";
                PauseScreen();
                continue;
            }

            customer->AddToCart(item->GetId(), qty, item->FinalPrice());
            cout << "Added to cart!\n";
            PauseScreen();
        }
    }
}

void Menus::ViewCart(Customer *customer)
{
    while (true)
    {
        ClearScreen();
        Cart &cart = customer->GetCart();

        if (cart.IsEmpty())
        {
            cout << "Cart is empty.\n";
            PauseScreen();
            return;
        }

        cart.PrintDetails();

        cout << "1. Remove item\n";
        cout << "2. Change quantity\n";
        cout << "3. Checkout\n";
        cout << "4. Clear cart\n";
        cout << "0. Back\n";

        int choice = GetIntInput("Enter choice: ");
        switch (choice)
        {
        case 1:
        {
            long itemId = GetIntInput("Enter item ID to remove: ");
            if (cart.RemoveItem(itemId))
                cout << "Item removed.\n";
            else
                cout << "Item not found.\n";
            PauseScreen();
            break;
        }
        case 2:
        {
            long itemId = GetIntInput("Enter item ID: ");
            int qty = GetIntInput("New quantity (0 to remove): ");
            cart.ChangeItemQuantity(itemId, qty);
            cout << "Updated.\n";
            PauseScreen();
            break;
        }
        case 3:
        {
            // make order id from time
            long orderId = (long)time(nullptr);
            time_t now = time(nullptr);
            string createdAt = string(ctime(&now));
            createdAt.pop_back();

            try
            {
                Order order = customer->Checkout(orderId, cart.GetRestaurantId(), createdAt);
                if (app.GetOrderDAO().Create(order))
                    cout << "Order placed successfully! Order ID: " << orderId << "\n";
                else
                    cout << "Failed to save order.\n";
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << "\n";
            }
            PauseScreen();
            return;
        }
        case 4:
            cart.Clear();
            cout << "Cart cleared.\n";
            PauseScreen();
            return;
        case 0:
            return;
        default:
            cout << "Invalid choice.\n";
            PauseScreen();
        }
    }
}

void Menus::OrderHistory(Customer *customer)
{
    ClearScreen();
    auto orders = app.GetOrderDAO().ReadByCustomer(customer->GetId());

    if (orders.empty())
    {
        cout << "No orders found.\n";
        PauseScreen();
        return;
    }

    cout << "===== Order History =====\n";
    for (const auto &order : orders)
        order->PrintDetails();

    PauseScreen();
}

void Menus::ManagerMenu(RestaurantManager *manager)
{
    while (true)
    {
        ClearScreen();
        cout << "===== Restaurant Manager Menu =====\n";
        cout << "Welcome, " << manager->GetUserName() << "\n";
        cout << "1. View/Edit Restaurant Info\n";
        cout << "2. Manage Menu Items\n";
        cout << "3. View Orders\n";
        cout << "0. Logout\n";
        cout << "===================================\n";

        int choice = GetIntInput("Enter choice: ");
        switch (choice)
        {
        case 1:
        {
            auto restaurant = app.GetRestaurantDAO().ReadById(manager->GetRestaurantId());
            if (!restaurant)
            {
                cout << "Restaurant not found.\n";
                PauseScreen();
                break;
            }

            ClearScreen();
            restaurant->PrintSummary();
            cout << "\n1. Edit Name\n2. Edit Address\n3. Edit Description\n0. Back\n";

            int edit = GetIntInput("Choice: ");
            if (edit == 1)
            {
                string val = GetStringInput("New name: ");
                restaurant->SetName(val);
                app.GetRestaurantDAO().Update(*restaurant);
                cout << "Updated.\n";
            }
            else if (edit == 2)
            {
                string val = GetStringInput("New address: ");
                restaurant->SetAddress(val);
                app.GetRestaurantDAO().Update(*restaurant);
                cout << "Updated.\n";
            }
            else if (edit == 3)
            {
                string val = GetStringInput("New description: ");
                restaurant->SetDescription(val);
                app.GetRestaurantDAO().Update(*restaurant);
                cout << "Updated.\n";
            }
            PauseScreen();
            break;
        }
        case 2:
            ManageMenu(manager);
            break;
        case 3:
            ViewOrders(manager);
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice.\n";
            PauseScreen();
        }
    }
}

void Menus::ManageMenu(RestaurantManager *manager)
{
    while (true)
    {
        ClearScreen();
        auto items = app.GetItemDAO().ReadByRestaurantId(manager->GetRestaurantId());

        cout << "===== Menu Items =====\n";
        if (items.empty())
            cout << "(No items)\n";
        else
            for (const auto &item : items)
                item->Print();

        cout << "1. Add Food\n";
        cout << "2. Add Drink\n";
        cout << "3. Edit Item Price\n";
        cout << "4. Edit Item Description\n";
        cout << "5. Toggle Item Availability\n";
        cout << "6. Delete Item\n";
        cout << "0. Back\n";

        int choice = GetIntInput("Enter choice: ");
        switch (choice)
        {
        case 1:
        {
            string name = GetStringInput("Name: ");
            string desc = GetStringInput("Description: ");
            double price = GetIntInput("Price: ");
            double tax = GetIntInput("Tax (%): ");
            float cal = GetIntInput("Calories: ");
            bool vegan = GetIntInput("Vegan? (1=yes, 0=no): ");
            int prep = GetIntInput("Prep time (min): ");

            auto food = make_unique<Food>(0, name, desc, price, tax, cal, vegan, prep);
            app.GetItemDAO().Create(*food, manager->GetRestaurantId());
            cout << "Food added.\n";
            PauseScreen();
            break;
        }
        case 2:
        {
            string name = GetStringInput("Name: ");
            string desc = GetStringInput("Description: ");
            double price = GetIntInput("Price: ");
            double tax = GetIntInput("Tax (%): ");
            cout << "Size: 1=Small 2=Medium 3=Large\n";
            int sz = GetIntInput("Size: ");
            DrinkSize size = (sz == 1) ? DrinkSize::Small : (sz == 3) ? DrinkSize::Large
                                                                      : DrinkSize::Medium;

            auto drink = make_unique<Drink>(0, name, desc, price, tax, size);
            app.GetItemDAO().Create(*drink, manager->GetRestaurantId());
            cout << "Drink added.\n";
            PauseScreen();
            break;
        }
        case 3:
        {
            long id = GetIntInput("Item ID: ");
            double newPrice = GetIntInput("New price: ");
            auto item = app.GetItemDAO().ReadById(id);
            if (!item)
            {
                cout << "Not found.\n";
                PauseScreen();
                break;
            }
            item->SetPrice(newPrice);
            app.GetItemDAO().Update(*item);
            cout << "Updated.\n";
            PauseScreen();
            break;
        }
        case 4:
        {
            long id = GetIntInput("Item ID: ");
            string desc = GetStringInput("New description: ");
            auto item = app.GetItemDAO().ReadById(id);
            if (!item)
            {
                cout << "Not found.\n";
                PauseScreen();
                break;
            }
            item->SetDescription(desc);
            app.GetItemDAO().Update(*item);
            cout << "Updated.\n";
            PauseScreen();
            break;
        }
        case 5:
        {
            long id = GetIntInput("Item ID: ");
            auto item = app.GetItemDAO().ReadById(id);
            if (!item)
            {
                cout << "Not found.\n";
                PauseScreen();
                break;
            }
            item->SetStatus(!item->GetStatus());
            app.GetItemDAO().Update(*item);
            cout << "Status toggled. Now: "
                 << (item->GetStatus() ? "Available" : "Unavailable") << "\n";
            PauseScreen();
            break;
        }
        case 6:
        {
            long id = GetIntInput("Item ID: ");
            if (app.GetItemDAO().Delete(id))
                cout << "Deleted.\n";
            else
                cout << "Not found.\n";
            PauseScreen();
            break;
        }
        case 0:
            return;
        default:
            cout << "Invalid choice.\n";
            PauseScreen();
        }
    }
}

void Menus::ViewOrders(RestaurantManager *manager)
{
    while (true)
    {
        ClearScreen();
        auto orders = app.GetOrderDAO().ReadByRestaurant(manager->GetRestaurantId());

        if (orders.empty())
        {
            cout << "No orders.\n";
            PauseScreen();
            return;
        }

        cout << "===== Orders =====\n";
        for (const auto &o : orders)
            o->PrintDetails();

        cout << "\n1. Update order status\n0. Back\n";
        int choice = GetIntInput("Choice: ");
        if (choice == 0)
            return;

        if (choice == 1)
        {
            long orderId = GetIntInput("Order ID: ");
            cout << "1. Preparing\n2. Ready To Send\n3. Delivered\n";
            int s = GetIntInput("New status: ");

            OrderStatus newStatus;
            if (s == 1)
                newStatus = OrderStatus::Preparing;
            else if (s == 2)
                newStatus = OrderStatus::ReadyToSend;
            else if (s == 3)
                newStatus = OrderStatus::Delivered;
            else
            {
                cout << "Invalid.\n";
                PauseScreen();
                continue;
            }

            if (app.GetOrderDAO().UpdateStatus(orderId, newStatus))
                cout << "Status updated.\n";
            else
                cout << "Failed.\n";
            PauseScreen();
        }
    }
}

void Menus::AdminMenu(Admin *admin)
{
    while (true)
    {
        ClearScreen();
        cout << "===== Admin Menu =====\n";
        cout << "Welcome, " << admin->GetUserName() << "\n";
        cout << "1. Register New Restaurant\n";
        cout << "2. Activate/Deactivate Restaurant\n";
        cout << "3. View Reports\n";
        cout << "0. Logout\n";
        cout << "======================\n";

        int choice = GetIntInput("Enter choice: ");
        switch (choice)
        {
        case 1:
            RegisterRestaurant();
            break;
        case 2:
            ToggleRestaurant();
            break;
        case 3:
            ViewReports();
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice.\n";
            PauseScreen();
        }
    }
}

void Menus::RegisterRestaurant()
{
    ClearScreen();
    cout << "===== Register Restaurant =====\n";
    string name = GetStringInput("Name: ");
    string addr = GetStringInput("Address: ");
    string phone = GetStringInput("Phone: ");
    string desc = GetStringInput("Description: ");

    Restaurant r(0, name, addr, true, phone, desc);
    if (app.GetRestaurantDAO().Create(r))
        cout << "Restaurant registered successfully.\n";
    else
        cout << "Failed to register restaurant.\n";
    PauseScreen();
}

void Menus::ToggleRestaurant()
{
    ClearScreen();
    auto restaurants = app.GetRestaurantDAO().ReadAll();

    if (restaurants.empty())
    {
        cout << "No restaurants found.\n";
        PauseScreen();
        return;
    }

    cout << "===== All Restaurants =====\n";
    for (const auto &r : restaurants)
        cout << "ID: " << r->GetId()
             << " | " << r->GetName()
             << " | Status: " << (r->GetIsActive() ? "Active" : "Inactive") << "\n";

    long id = GetIntInput("Enter restaurant ID to toggle: ");
    auto r = app.GetRestaurantDAO().ReadById(id);
    if (!r)
    {
        cout << "Restaurant not found.\n";
        PauseScreen();
        return;
    }

    r->SetIsActive(!r->GetIsActive());
    app.GetRestaurantDAO().Update(*r);
    cout << "Restaurant is now: " << (r->GetIsActive() ? "Active" : "Inactive") << "\n";
    PauseScreen();
}

void Menus::ViewReports()
{
    ClearScreen();
    cout << "===== System Reports =====\n";

    auto allUsers = app.GetUserDAO().ReadAll();
    auto allRestaurants = app.GetRestaurantDAO().ReadAll();

    // collecting all orders
    vector<Order> allOrders;
    for (const auto &r : allRestaurants)
    {
        auto orders = app.GetOrderDAO().ReadByRestaurant(r->GetId());
        for (auto &o : orders)
            allOrders.push_back(*o);
    }

    // status
    Admin tempAdmin;
    cout << "Total Users        : " << allUsers.size() << "\n";
    cout << "Active Users       : " << tempAdmin.GetActiveUsersCount(allUsers) << "\n";
    cout << "Total Restaurants  : " << allRestaurants.size() << "\n";
    cout << "Active Restaurants : " << tempAdmin.GetActiveRestaurantsCount(allRestaurants) << "\n";
    cout << "Delivered Orders   : " << tempAdmin.GetDeliveredOrdersCount(allOrders) << "\n";
    cout << "Total Orders       : " << tempAdmin.GetTotalOrdersCount(allOrders) << "\n";
    cout << "Total Sales        : " << tempAdmin.GetTotalSales(allOrders) << " Toman\n";

    PauseScreen();
}