#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>

enum class Role
{
    Customer,
    Admin,
    RestaurantManager
};

class User
{
private:
    long id;
    std::string UserName;
    std::string password;
    std::string Phone_Number;
    std::string Address;
    Role role; // customer , manager , admin
    bool Is_Active;
    std::string Created_At;

public:
    // constructor
    User(); // default cons
    User(long, std::string, std::string, std::string, std::string, Role, std::string);

    // distructor
    virtual ~User();

    // Setter
    void SetId(long);
    void SetUserName(std::string);
    void SetPassword(std::string);
    void SetPhone_Number(std::string);
    void SetAddress(std::string);
    void SetRole(Role);
    void SetIs_Active(bool);
    void SetCreated_At(std::string);

    // Getter
    long GetId() const;
    std::string GetUserName() const;
    std::string GetPhone_Number() const;
    std::string GetAddress() const;
    Role GetRole() const;
    bool GetIs_Active() const;
    std::string GetCreated_At() const;

    virtual void Print_Details() const;
};

#endif