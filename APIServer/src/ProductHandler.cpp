#include "ProductHandler.h"
#include <iostream>
#include <sstream>

ProductHandler::ProductHandler(MYSQL *connection) : conn(connection) {}

std::string ProductHandler::handleGetCategories() {
    std::stringstream result;
    result << "[";
    
    if (mysql_query(conn, "SELECT name FROM categories") == 0) {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        bool first = true;
        while ((row = mysql_fetch_row(res))) {
            if (!first) result << ", ";
            result << "\"" << row[0] << "\"";
            first = false;
        }
        mysql_free_result(res);
    }
    
    result << "]";
    return result.str();
}

std::string ProductHandler::handleGetProducts() {
    std::stringstream result;
    result << "[";
    
    if (mysql_query(conn, "SELECT id, name FROM products") == 0) {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        bool first = true;
        while ((row = mysql_fetch_row(res))) {
            if (!first) result << ", ";
            result << "{\"id\":" << row[0] << ", \"name\":\"" << row[1] << "\"}";
            first = false;
        }
        mysql_free_result(res);
    }
    
    result << "]";
    return result.str();
}
