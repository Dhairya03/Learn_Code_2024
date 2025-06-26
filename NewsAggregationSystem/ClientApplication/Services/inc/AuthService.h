#pragma once
#include "../../Core/inc/Client.h"
#include <string>

class AuthService {
public:
    explicit AuthService(Client& client);
    std::string login(const std::string& email, const std::string& password);
    std::string signup(const std::string& username, const std::string& email, const std::string& password);

private:
    Client& client;
};
