#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>

using namespace std;

class User
{
private:
    string UserName;
    string password;
    string Phone_Number;
    string Address;
    string role; // customer , manager , admin
    bool Is_Active;
    string Created_At;

public:
    // constructor
    User(); // default cons
    User(string, string, string, string, string, string);

    // distructor
    virtual ~User();

    // Setter
    void SetUserName(string);
    void SetPassword(string);
    void SetPhone_Number(string);
    void SetAddress(string);
    void SetRole(string);
    void SetIs_Active(bool);
    void SetCreated_At(string);

    // Getter
    string GetUserName() const;
    string GetPhone_Number() const;
    string GetAddress() const;
    string GetRole() const;
    bool GetIs_Active() const;
    string GetCreated_At() const;

    virtual void Print_Details() const;
};

#endif