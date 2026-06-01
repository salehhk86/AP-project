#include "User.hpp"
using namespace std;

// constructor
User::User()
    : id(-1), UserName(""), password(""), Phone_Number(""), Address(""),
      role(Role::Customer), Is_Active(false), Created_At("") {} // default cons

User::User(long i, string u, string p, string ph, string a, Role r, string c)
    : id(i), UserName(u), password(p), Phone_Number(ph), Address(a), role(r), Is_Active(true), Created_At(c) {}

// distructor
User::~User() = default;

// Setter
void User::SetId(long i) { id = i; }
void User::SetUserName(string u) { UserName = u; }
void User::SetPassword(string p) { password = p; }
void User::SetPhone_Number(string p) { Phone_Number = p; }
void User::SetAddress(string a) { Address = a; }
void User::SetRole(Role r) { role = r; }
void User::SetIs_Active(bool a) { Is_Active = a; }
void User::SetCreated_At(string c) { Created_At = c; }

// getter
long User::GetId() const { return id; }
string User::GetUserName() const { return UserName; }
string User::GetPhone_Number() const { return Phone_Number; }
string User::GetAddress() const { return Address; }
Role User::GetRole() const { return role; }
bool User::GetIs_Active() const { return Is_Active; }
string User::GetCreated_At() const { return Created_At; }

void User::Print_Details() const
{
    string ac = (Is_Active) ? "active" : "inactive";

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
        break;
    }

    cout << "\nid : " << id
         << "\nusername : " << UserName
         << "\nrole : " << roleS
         << "\nPhone number : " << Phone_Number
         << "\nAddress : " << Address
         << "\nactivation status : " << ac
         << "\ncreated at : " << Created_At
         << endl;
}
