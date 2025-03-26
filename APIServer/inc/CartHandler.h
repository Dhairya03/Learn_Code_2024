#ifndef CART_HANDLER_H
#define CART_HANDLER_H

#include <mysql/mysql.h>
#include <crow_all.h>

class CartHandler {
private:
    MYSQL *conn;
public:
    CartHandler(MYSQL *connection);
    std::string handleAddToCart(const crow::request& req);
    std::string handleViewCart();
    std::string handleBuyNow();
};

#endif
