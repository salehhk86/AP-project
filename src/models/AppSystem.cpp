#include "AppSystem.hpp"
#include <iostream>
#include <ctime>

using namespace std;

AppSystem::AppSystem(UserDAO &u, RestaurantDAO &r, ItemDAO &i, OrderDAO &o)
    : userDAO(u), restaurantDAO(r), itemDAO(i), orderDAO(o),
      currentUserOwner(nullptr), currentUser(nullptr) {}

bool AppSystem::SignUp(const string &username, const string &pass,
                       const string &phone, const string &address,
                       Role role, long restaurantId)
{
    if (userDAO.ReadByUsername(username))
    {
        cout << "Username already exists.\n";
        return false;
    }

    time_t now = time(nullptr);
    string createdAt = string(ctime(&now));
    createdAt.pop_back(); // delete newline

    unique_ptr<User> newUser;

    switch (role)
    {
    case Role::Customer:
        newUser = make_unique<Customer>(0, username, pass, phone, address, createdAt);
        break;

    case Role::Admin:
        newUser = make_unique<Admin>(0, username, pass, phone, address, createdAt);
        break;

    case Role::RestaurantManager:
        newUser = make_unique<RestaurantManager>(
            0, username, pass, phone, address, createdAt, restaurantId);
        break;

    default:
        cout << "Invalid role!\n";
        return false;
    }

    if (!userDAO.Create(*newUser))
    {
        cout << "Registration failed.\n";
        return false;
    }

    cout << "User registered successfully.\n";
    return true;
}

User *AppSystem::SignIn(const string &username, const string &pass)
{
    auto user = userDAO.ReadByUsernameForLogin(username);

    if (!user)
    {
        cout << "Username not found or account is deactivated.\n";
        return nullptr;
    }

    if (!user->CheckPassword(pass))
    {
        cout << "Invalid password.\n";
        return nullptr;
    }

    currentUserOwner = move(user);   
    currentUser = currentUserOwner.get(); // pointer to the user for quick access

    cout << "Login successful. Welcome, " << currentUser->GetUserName() << "!\n";
    return currentUser;
}

void AppSystem::SignOut()
{
    if (currentUser)
    {
        currentUserOwner.reset(); // memory cleanup
        currentUser = nullptr;
        cout << "Logged out successfully.\n";
    }
}

User *AppSystem::GetCurrentUser() const { return currentUser; }