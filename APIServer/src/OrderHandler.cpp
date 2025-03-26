#include "OrderHandler.h"
#include <iostream>
#include <sstream>

OrderHandler::OrderHandler(MYSQL *connection) : conn(connection) {}

std::string OrderHandler::handleOrderHistory() {
    std::stringstream result;
    result << "[";
    
    if (mysql_query(conn, "SELECT order_id, product_id, quantity FROM orders") == 0) {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        bool first = true;
        while ((row = mysql_fetch_row(res))) {
            if (!first) result << ", ";
            result << "{\"order_id\":" << row[0] << ", \"product_id\": " << row[1] << ", \"quantity\": " << row[2] << "}";
            first = false;
        }
        mysql_free_result(res);
    }
    
    result << "]";
    return result.str();
}
