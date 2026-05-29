#include "User.hpp"

// constructor
User::User() {} // default cons

User::User(string u, string p, string ph, string a, string r, string c)
    : UserName(u), password(p), Phone_Number(ph), Address(a), role(r), Is_Active(true), Created_At(c) {}

// distructor
User::~User() = default;

// Setter
void User::SetUserName(string u) { UserName = u; }
void User::SetPassword(string p) { password = p; }
void User::SetPhone_Number(string p) { Phone_Number = p; }
void User::SetAddress(string a) { Address = a; }
void User::SetRole(string r) { role = r; }
void User::SetIs_Active(bool a) { Is_Active = a; }
void User::SetCreated_At(string c) { Created_At = c; }

// getter
string User::GetUserName() const { return UserName; }
string User::GetPhone_Number() const { return Phone_Number; }
string User::GetAddress() const { return Address; }
string User::GetRole() const { return role; }
bool User::GetIs_Active() const { return Is_Active; }
string User::GetCreated_At() const { return Created_At; }

void User::Print_Details() const
{
    string ac = (Is_Active) ? "active" : "inactive";

    cout << "\nusername : " << UserName
         << "\nrole : " << role
         << "\nPhone number : " << Phone_Number
         << "\nAddress : " << Address
         << "\nactivation status : " << ac
         << "\ncreated at : " << Created_At
         << endl;
}
