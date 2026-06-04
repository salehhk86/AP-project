#include "User.hpp"
#include <iostream>

using namespace std;

// constructor
User::User()
    : id(-1), userName(""), password(""), phoneNumber(""), address(""),
      role(Role::Customer), isActive(false), createdAt("") {} // default cons

User::User(long i, const string &u, const string &p, const string &ph, const string &a, Role r, const string &c)
    : id(i), userName(u), password(p), phoneNumber(ph), address(a), role(r), isActive(true), createdAt(c) {}

// Setter
void User::SetId(long i) { id = i; }
void User::SetUserName(const string &u) { userName = u; }
void User::SetPassword(const string &p) { password = p; }
void User::SetPhone_Number(const string &p) { phoneNumber = p; }
void User::SetAddress(const string &a) { address = a; }
void User::SetRole(Role r) { role = r; }
void User::SetIs_Active(bool a) { isActive = a; }
void User::SetCreated_At(const string &c) { createdAt = c; }

// getter
long User::GetId() const { return id; }
string User::GetUserName() const { return userName; }
string User::GetPhone_Number() const { return phoneNumber; }
string User::GetAddress() const { return address; }
Role User::GetRole() const { return role; }
bool User::GetIs_Active() const { return isActive; }
string User::GetCreated_At() const { return createdAt; }

bool User::CheckPassword(const string &input) const { return password == input; }

void User::Print_Details() const
{
    string ac = (isActive) ? "active" : "inactive";

    string roleS;

    switch (role)
    {
    case Role::Customer:
        roleS = "Customer";
        break;

    case Role::Admin:
        roleS = "Admin";
        break;

    case Role::RestaurantManager:
        roleS = "RestaurantManager";
        break;

    default:
        roleS = "Unknown";
        break;
    }

    cout << "\nid : " << id
         << "\nusername : " << userName
         << "\nrole : " << roleS
         << "\nPhone number : " << phoneNumber
         << "\nAddress : " << address
         << "\nactivation status : " << ac
         << "\ncreated at : " << createdAt
         << endl;
}
