#include "AuthHandler.h"
#include <iostream>

AuthHandler::AuthHandler(MYSQL *connection) : conn(connection) {}

std::string AuthHandler::handleSignup(const crow::request& req) {
    auto x = crow::json::load(req.body);
    if (!x) return "Invalid JSON";

    std::string query = "INSERT INTO users (username, password) VALUES ('" +
                         std::string(x["username"].s()) + "', '" +
                         std::string(x["password"].s()) + "')";
    
    if (mysql_query(conn, query.c_str()) == 0) {
        return "Signup successful";
    } else {
        return "Signup failed";
    }
}

std::string AuthHandler::handleLogin(const crow::request& req) {
    auto x = crow::json::load(req.body);
    if (!x) return "Invalid JSON";

    std::string query = "SELECT * FROM users WHERE username='" + std::string(x["username"].s()) + "' AND password='" + std::string(x["password"].s()) + "'";
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES *res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) > 0) {
            mysql_free_result(res);
            return "Login successful";
        } else {
            return "Invalid credentials";
        }
    }
    return "Login failed";
}