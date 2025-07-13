#include "controllers/inc/CategoryController.h"
#include "lib/json/json.hpp"
#include <iostream>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using json = nlohmann::json;

crow::response CategoryController::getAllCategories(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[CategoryController] getAllCategories called" << std::endl;
    
    try {
        auto conn = dbConn->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT id, name FROM categories ORDER BY name"));
        
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        
        json categoriesArray = json::array();
        while (res->next()) {
            json category = {
                {"id", res->getInt("id")},
                {"name", res->getString("name")}
            };
            categoriesArray.push_back(category);
        }
        
        json response = {
            {"success", true},
            {"data", categoriesArray}
        };
        
        std::cout << "[CategoryController] getAllCategories success, returned " << categoriesArray.size() << " categories" << std::endl;
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[CategoryController] getAllCategories error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
}

crow::response CategoryController::getCategoryById(const crow::request& req, std::shared_ptr<DBConnection> dbConn, int categoryId) {
    std::cout << "[CategoryController] getCategoryById called for ID: " << categoryId << std::endl;
    
    try {
        auto conn = dbConn->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT id, name FROM categories WHERE id = ?"));
        stmt->setInt(1, categoryId);
        
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        
        if (res->next()) {
            json category = {
                {"id", res->getInt("id")},
                {"name", res->getString("name")}
            };
            
            json response = {
                {"success", true},
                {"data", category}
            };
            return crow::response(200, response.dump());
        } else {
            json response = {
                {"success", false},
                {"message", "Category not found"}
            };
            return crow::response(404, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[CategoryController] getCategoryById error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
}

crow::response CategoryController::getCategoryByName(const crow::request& req, std::shared_ptr<DBConnection> dbConn, const std::string& categoryName) {
    std::cout << "[CategoryController] getCategoryByName called for name: " << categoryName << std::endl;
    
    try {
        auto conn = dbConn->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT id, name FROM categories WHERE name = ?"));
        stmt->setString(1, categoryName);
        
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        
        if (res->next()) {
            json category = {
                {"id", res->getInt("id")},
                {"name", res->getString("name")}
            };
            
            json response = {
                {"success", true},
                {"data", category}
            };
            return crow::response(200, response.dump());
        } else {
            json response = {
                {"success", false},
                {"message", "Category not found"}
            };
            return crow::response(404, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[CategoryController] getCategoryByName error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
} 