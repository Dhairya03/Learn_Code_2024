#include "../inc/UserService.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

UserService::UserService(Client& client) : client(client) {}

bool UserService::registerUser(const std::string& username, const std::string& email, const std::string& password) {
    try {
        json body = {
            {"username", username},
            {"email", email},
            {"password", password}
        };
        
        std::string response = client.post("/auth/register", body.dump());
        auto responseJson = json::parse(response);
        
        return responseJson.contains("success") && responseJson["success"].get<bool>();
    } catch (const std::exception& e) {
        std::cerr << "Error registering user: " << e.what() << std::endl;
        return false;
    }
}

nlohmann::json UserService::loginUser(const std::string& username, const std::string& password) {
    try {
        json body = {
            {"username", username},
            {"password", password}
        };
        
        std::string response = client.post("/auth/login", body.dump());
        return json::parse(response);
    } catch (const std::exception& e) {
        std::cerr << "Error logging in user: " << e.what() << std::endl;
        return json();
    }
}

bool UserService::updateUser(int userId, const std::string& username, const std::string& email) {
    try {
        json body = {
            {"username", username},
            {"email", email}
        };
        
        std::string endpoint = "/users/" + std::to_string(userId);
        std::string response = client.put(endpoint, body.dump());
        auto responseJson = json::parse(response);
        
        return responseJson.contains("success") && responseJson["success"].get<bool>();
    } catch (const std::exception& e) {
        std::cerr << "Error updating user: " << e.what() << std::endl;
        return false;
    }
}

bool UserService::deleteUser(int userId) {
    try {
        std::string endpoint = "/users/" + std::to_string(userId);
        std::string response = client.delete_(endpoint);
        auto responseJson = json::parse(response);
        
        return responseJson.contains("success") && responseJson["success"].get<bool>();
    } catch (const std::exception& e) {
        std::cerr << "Error deleting user: " << e.what() << std::endl;
        return false;
    }
}

nlohmann::json UserService::getUserById(int userId) {
    try {
        std::string endpoint = "/users/" + std::to_string(userId);
        std::string response = client.get(endpoint);
        return json::parse(response);
    } catch (const std::exception& e) {
        std::cerr << "Error getting user: " << e.what() << std::endl;
        return json();
    }
}

