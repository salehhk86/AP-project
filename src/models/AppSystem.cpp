#include "AppSystem.hpp"
#include <iostream>

using namespace std;

AppSystem::AppSystem() : currentUser(nullptr) {}

bool AppSystem::UsernameExists(const string &username) const
{
    for (const auto &a : users)
        if (a->GetUserName() == username)
            return true;
    return false;
}

bool AppSystem::SignUp(long id, const string &username, const string &pass, const string &phone, const string &address, Role role, const string &cratedat)
{
    if (UsernameExists(username))
    {
        cout << "Username already exists\n";
        return false;
    }

    unique_ptr<User> newuser;
    switch (role)
    {
    case Role::Admin:
        newuser = make_unique<Admin>(id, username, pass, phone, address, cratedat);
        break;

    case Role::RestaurantManager:
        newuser = make_unique<RestaurantManager>(id, username, pass, phone, address, cratedat);
        break;

    case Role::Customer:
        newuser = make_unique<Customer>(id, username, pass, phone, address, cratedat);
        break;

    default:
        break;
    }
    if (!newuser)
    {
        cout << "Invalid  role!\n";
        return false;
    }
    users.push_back(move(newuser));

    cout << "User registered successfully\n";
    return true;
}

User *AppSystem::SignIn(const string &username, const string &pass)
{
    int sw = 0;
    for (const auto &user : users)
    {
        if (username == user->GetUserName())
        {
            sw = 1;
            if (user->CheckPassword(pass))
            {
                currentUser = user.get(); // method from unique ptr
                cout << "Login successful\n";
                return currentUser;
            }
        }
    }
    if (sw)
        cout << "Invalid password\n";
    else
        cout << "Username not found!\n";
    return nullptr;
}

void AppSystem::SignOut()
{
    currentUser = nullptr;
    cout << "Logged out\n";
}

User *AppSystem::GetCurrentUser() const { return currentUser; }