#include "CartHandler.h"
#include <iostream>
#include <sstream>

CartHandler::CartHandler(MYSQL *connection) : conn(connection) {}

std::string CartHandler::handleAddToCart(const crow::request& req) {
    auto x = crow::json::load(req.body);
    if (!x) return "Invalid JSON";
    
    std::string query = "INSERT INTO cart (user_id, product_id, quantity) VALUES (" +
                         std::to_string(x["user_id"].i()) + ", " +
                         std::to_string(x["product_id"].i()) + ", " +
                         std::to_string(x["quantity"].i()) + ")";
    
    if (mysql_query(conn, query.c_str()) == 0) {
        return "Product added to cart";
    } else {
        return "Failed to add product to cart";
    }
}

std::string CartHandler::handleViewCart() {
    std::stringstream result;
    result << "[";
    
    if (mysql_query(conn, "SELECT product_id, quantity FROM cart") == 0) {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        bool first = true;
        while ((row = mysql_fetch_row(res))) {
            if (!first) result << ", ";
            result << "{\"product_id\":" << row[0] << ", \"quantity\": " << row[1] << "}";
            first = false;
        }
        mysql_free_result(res);
    }
    
    result << "]";
    return result.str();
}

std::string CartHandler::handleBuyNow() {
    if (mysql_query(conn, "INSERT INTO orders (user_id, product_id, quantity) SELECT user_id, product_id, quantity FROM cart") == 0) {
        mysql_query(conn, "DELETE FROM cart");
        return "Order placed successfully";
    }
    return "Failed to place order";
}
