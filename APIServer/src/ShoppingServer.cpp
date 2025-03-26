#include "ShoppingServer.h"
#include "AuthHandler.h"
#include "ProductHandler.h"
#include "CartHandler.h"
#include "OrderHandler.h"
#include <iostream>

ShoppingServer::ShoppingServer() {
    connectDatabase();
    authHandler = new AuthHandler(conn);
    productHandler = new ProductHandler(conn);
    cartHandler = new CartHandler(conn);
    orderHandler = new OrderHandler(conn);
}

ShoppingServer::~ShoppingServer() {
    delete authHandler;
    delete productHandler;
    delete cartHandler;
    delete orderHandler;
    mysql_close(conn);
}

void ShoppingServer::connectDatabase() {
    conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, "localhost", "root", "password", "shopping_db", 3306, NULL, 0);
        if (conn) {
            std::cout << "Database connected successfully!" << std::endl;
        } else {
            std::cerr << "Database connection failed!" << std::endl;
        }
    }
}

void ShoppingServer::startServer() {
    crow::SimpleApp app;
    
    CROW_ROUTE(app, "/signup").methods(crow::HTTPMethod::POST)([this](const crow::request& req){ return crow::response(authHandler->handleSignup(req)); });
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([this](const crow::request& req){ return crow::response(authHandler->handleLogin(req)); });
    
    CROW_ROUTE(app, "/menu").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x)
            return "Invalid JSON";
        std::string userId = x["user_id"].s();
        if (userId.empty())
            return "Unauthorized access";
        return "1. View Categories\n2. View Products\n3. Add to Cart\n4. View Cart\n5. Buy Now\n6. Order History\n7. Logout";
    });
    
    CROW_ROUTE(app, "/categories").methods(crow::HTTPMethod::GET)([this](){ return crow::response(productHandler->handleGetCategories()); });
    CROW_ROUTE(app, "/products").methods(crow::HTTPMethod::GET)([this](){ return crow::response(productHandler->handleGetProducts()); });
    
    CROW_ROUTE(app, "/cart/add").methods(crow::HTTPMethod::POST)([this](const crow::request& req){ return crow::response(cartHandler->handleAddToCart(req)); });
    CROW_ROUTE(app, "/cart").methods(crow::HTTPMethod::GET)([this](){ return crow::response(cartHandler->handleViewCart()); });
    CROW_ROUTE(app, "/cart/buy").methods(crow::HTTPMethod::POST)([this](){ return crow::response(cartHandler->handleBuyNow()); });
    
    CROW_ROUTE(app, "/orders").methods(crow::HTTPMethod::GET)([this](){ return crow::response(orderHandler->handleOrderHistory()); });
    
    app.port(8080).multithreaded().run();
}
