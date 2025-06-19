#include "controllers/inc/UserController.h"
#include "services/inc/UserService.h"
#include "lib/json/json.hpp"

using json = nlohmann::json;

crow::response UserController::signup(const crow::request& req) {
    try {
        auto body = json::parse(req.body);
        std::string username = body["username"];
        std::string email = body["email"];
        std::string password = body["password"];

        UserService service;
        if (service.signup(username, email, password)) {
            return crow::response(200, "User created successfully");
        } else {
            return crow::response(400, "Signup failed");
        }
    } catch (...) {
        return crow::response(500, "Internal Server Error");
    }
}
