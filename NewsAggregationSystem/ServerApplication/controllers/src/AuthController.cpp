#include "controllers/inc/AuthController.h"
#include "services/inc/AuthService.h"
#include "lib/json/json.hpp"
#include <iostream>

using json = nlohmann::json;

crow::response AuthController::login(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    try {
        std::cout << "[AuthController] Received login request\n";
        std::cout << "[AuthController] Request body: " << req.body << "\n";
        
        auto body = json::parse(req.body);
        
        if (!body.contains("email") || !body.contains("password")) {
            std::cout << "[AuthController] Missing email or password\n";
            json response = {
                {"success", false},
                {"message", "Missing email or password"}
            };
            crow::response res(400, response.dump());
            res.add_header("Content-Type", "application/json");
            return res;
        }

        std::string email = body["email"];
        std::string password = body["password"];
        
        std::cout << "[AuthController] Attempting login for email: " << email << "\n";

        AuthService service(dbConn);
        auto userOpt = service.login(email, password);

        if (userOpt.has_value()) {
            auto user = userOpt.value();
            std::cout << "[AuthController] Login successful for user: " << user.username << "\n";
            
            json response = {
                {"success", true},
                {"message", "Login successful"},
                {"user_id", user.id},
                {"username", user.username},
                {"role", user.role}
            };
            
            crow::response res(200, response.dump());
            res.add_header("Content-Type", "application/json");
            return res;
        } else {
            std::cout << "[AuthController] Invalid credentials for email: " << email << "\n";
            json response = {
                {"success", false},
                {"message", "Invalid credentials"}
            };
            
            crow::response res(401, response.dump());
            res.add_header("Content-Type", "application/json");
            return res;
        }

    } catch (const json::parse_error& e) {
        std::cerr << "[AuthController] JSON parse error: " << e.what() << "\n";
        json response = {
            {"success", false},
            {"message", "Invalid JSON format"}
        };
        crow::response res(400, response.dump());
        res.add_header("Content-Type", "application/json");
        return res;
        
    } catch (const std::exception& e) {
        std::cerr << "[AuthController] Internal server error: " << e.what() << "\n";
        json response = {
            {"success", false},
            {"message", "Internal Server Error"}
        };
        crow::response res(500, response.dump());
        res.add_header("Content-Type", "application/json");
        return res;
    }
}

crow::response AuthController::logout(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    try {
        std::cout << "[AuthController] Logout request received\n";
        
        AuthService service(dbConn);
        bool success = service.logout();

        json response;
        crow::response res;
        
        if (success) {
            response = {
                {"success", true},
                {"message", "Logout successful"}
            };
            res = crow::response(200, response.dump());
        } else {
            response = {
                {"success", false},
                {"message", "Logout failed"}
            };
            res = crow::response(500, response.dump());
        }
        
        res.add_header("Content-Type", "application/json");
        return res;
        
    } catch (const std::exception& e) {
        std::cerr << "[AuthController] Logout error: " << e.what() << "\n";
        json response = {
            {"success", false},
            {"message", "Internal Server Error"}
        };
        crow::response res(500, response.dump());
        res.add_header("Content-Type", "application/json");
        return res;
    }
}