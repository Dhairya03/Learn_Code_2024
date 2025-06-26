#ifndef USER_H
#define USER_H

#include <string>

enum class UserRole {
    USER,
    ADMIN
};

class User {
public:
    int id = 0;
    std::string username;
    std::string email;
    std::string password;  
    UserRole role = UserRole::USER;

    static UserRole parseRole(const std::string& roleStr) {
        if (roleStr == "admin") return UserRole::ADMIN;
        return UserRole::USER;
    }

    static std::string roleToString(UserRole role) {
        return role == UserRole::ADMIN ? "admin" : "user";
    }
};


#endif 
