#include "controllers/inc/UserController.h"
#include "services/inc/UserService.h"
#include "lib/json/json.hpp"

using json = nlohmann::json;

crow::response UserController::signup(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
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
            return crow::response(200, response.dump());
        } else {
            json response = {
                {"success", false},
                {"message", "Signup failed"}
            };
            return crow::response(400, response.dump());
        }
    } catch (...) {
        json response = {
            {"success", false},
            {"message", "Internal Server Error"}
        };
        return crow::response(500, response.dump());
    }
}
