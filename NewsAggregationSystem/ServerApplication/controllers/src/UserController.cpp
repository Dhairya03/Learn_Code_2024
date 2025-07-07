#include "controllers/inc/UserController.h"
#include "services/inc/UserService.h"
#include "lib/json/json.hpp"
#include <iostream>

using json = nlohmann::json;

crow::response UserController::signup(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[UserController] signup called" << std::endl;
    try {
        auto body = json::parse(req.body);
        std::string username = body["username"];
        std::string email = body["email"];
        std::string password = body["password"];
        UserService service(dbConn);
        if (service.signup(username, email, password)) {
            json response = {
                {"success", true},
                {"message", "User created successfully"}
            };
            std::cout << "[UserController] signup success" << std::endl;
            return crow::response(200, response.dump());
        } else {
            json response = {
                {"success", false},
                {"message", "Signup failed"}
            };
            std::cerr << "[UserController] signup failed" << std::endl;
            return crow::response(400, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[UserController] signup error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", e.what()}
        };
        return crow::response(500, response.dump());
    } catch (...) {
        std::cerr << "[UserController] signup unknown error" << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal Server Error"}
        };
        return crow::response(500, response.dump());
    }
}
