#ifndef SHOPPING_SERVER_H
#define SHOPPING_SERVER_H

#include <mysql/mysql.h>
#include <crow_all.h>
#include "AuthHandler.h"
#include "ProductHandler.h"
#include "CartHandler.h"
#include "OrderHandler.h"

class ShoppingServer {
private:
    MYSQL* conn;
    crow::SimpleApp app;
    AuthHandler authHandler;
    ProductHandler sproductHandler;
    CartHandler cartHandler;
    OrderHandler orderHandler;

    void connectDatabase();
    void setupRoutes();

public:
    ShoppingServer();
    ~ShoppingServer();
    void startServer();
};

#endif
