#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <string>
#include <nlohmann/json.hpp>
#include "../../Core/inc/Client.h"

class UserService {
private:
    Client& client;

public:
    UserService(Client& client);
    
    bool registerUser(const std::string& username, const std::string& email, const std::string& password);
    nlohmann::json loginUser(const std::string& username, const std::string& password);
    bool updateUser(int userId, const std::string& username, const std::string& email);
    bool deleteUser(int userId);
    nlohmann::json getUserById(int userId);
};

#endif

