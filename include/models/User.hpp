#ifndef USER_HPP
#define USER_HPP

#include <string>

enum class Role
{
    Customer,
    Admin,
    RestaurantManager
};

class User
{
protected:
    long id;
    std::string userName;
    std::string password;
    std::string phoneNumber;
    std::string address;
    Role role; // customer , manager , admin
    bool isActive;
    std::string createdAt;

public:
    // constructor
    User(); // default cons
    User(long, const std::string &, const std::string &, const std::string &, const std::string &, Role, const std::string &);

    // distructor
    virtual ~User() = default;

    // Setter
    void SetId(long);
    void SetUserName(const std::string &);
    void SetPassword(const std::string &);
    void SetPhone_Number(const std::string &);
    void SetAddress(const std::string &);
    void SetRole(Role);
    void SetIs_Active(bool);
    void SetCreated_At(const std::string &);

    // Getter
    long GetId() const;
    std::string GetUserName() const;
    std::string GetPassword() const; // only for data base
    std::string GetPhone_Number() const;
    std::string GetAddress() const;
    Role GetRole() const;
    bool GetIs_Active() const;
    std::string GetCreated_At() const;

    bool CheckPassword(const std::string &) const;

    virtual void Print_Details() const;
};

#endif