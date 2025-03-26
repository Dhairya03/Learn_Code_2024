#include "ClientApplication.h"
#include <iostream>

ClientApplication::ClientApplication() : isLoggedIn(false) {}

void ClientApplication::run() {
    while (!isLoggedIn) {
        std::cout << "1. Login\n2. Signup\n3. Exit\nChoose an option: ";
        int choice;
        std::cin >> choice;
        switch (choice) {
            case 1:
                handleLogin();
                break;
            case 2:
                handleSignup();
                break;
            case 3:
                return;
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    }
    showMainMenu();
}

void ClientApplication::handleLogin() {
    if (userHandler.login()) {
        isLoggedIn = true;
    }
}

void ClientApplication::handleSignup() {
    userHandler.signup();
}

void ClientApplication::showMainMenu() {
    while (true) {
        std::cout << "1. Browse Categories\n2. View Cart\n3. View Order History\n4. Logout\nChoose an option: ";
        int choice;
        std::cin >> choice;
        switch (choice) {
            case 1:
                shoppingClient.browseCategories();
                break;
            case 2:
                cartHandler.viewCart();
                break;
            case 3:
                orderHandler.handleOrderHistory();
                break;
            case 4:
                isLoggedIn = false;
                return;
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    }
}
