#ifndef APPSYSTEM_HPP
#define APPSYSTEM_HPP

#include <vector>
#include <memory>
#include <string>
#include "User.hpp"
#include "Admin.hpp"
#include "Customer.hpp"
#include "RestaurantManager.hpp"

class AppSystem
{

private:
    std::vector<std::unique_ptr<User>> users;
    User *currentUser;

    bool UsernameExists(const std::string &) const;

public:
    // constructor
    AppSystem();

    bool SignUp(long, const std::string &, const std::string &, const std::string &, const std::string &, Role, const std::string &);

    User *SignIn(const std::string &, const std::string &);

    void SignOut();

    User *GetCurrentUser() const;
};

#endif