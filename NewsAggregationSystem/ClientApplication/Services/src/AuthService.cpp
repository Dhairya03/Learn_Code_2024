#include "../../Services/inc/AuthService.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

AuthService::AuthService(Client& c) : client(c) {}

std::string AuthService::login(const std::string& email, const std::string& password) {
    json body = {
        {"email", email},
        {"password", password}
    };
    return client.post("/login", body.dump());
}

std::string AuthService::signup(const std::string& username, const std::string& email, const std::string& password) {
    json body = {
        {"username", username},
        {"email", email},
        {"password", password}
    };
    return client.post("/signup", body.dump());
}
