#ifndef CLIENT_APPLICATION_H
#define CLIENT_APPLICATION_H

#include "UserHandler.h"
#include "ShoppingClient.h"
#include "CartHandler.h"
#include "OrderHandler.h"

class ClientApplication {
private:
    UserHandler userHandler;
    ShoppingClient shoppingClient;
    CartHandler cartHandler;
    OrderHandler orderHandler;
    bool isLoggedIn;

    void showMainMenu();
    void handleLogin();
    void handleSignup();

public:
    ClientApplication();
    void run();
};

#endif
