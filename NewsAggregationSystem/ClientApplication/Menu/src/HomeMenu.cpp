#include "../../Menu/inc/HomeMenu.h"
#include "../../Menu/inc/AdminMenu.h"
#include "../../Menu/inc/UserMenu.h"
#include "../../Services/inc/AuthService.h"
#include "../../Utils/inc/Validator.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

HomeMenu::HomeMenu(Client& httpClient, Session& userSession) : httpClient(httpClient), userSession(userSession) {}

void HomeMenu::display() {
    while (true) {
        std::cout << "\nWelcome to the News Aggregator Application\n"
                  << "1. Login\n"
                  << "2. Sign up\n"
                  << "3. Exit\n>> ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: displayLogin(); break;
            case 2: displaySignup(); break;
            case 3: exitApplication(); break;
            default: std::cout << "Invalid option.\n";
        }
    }
}

void HomeMenu::displayLogin() {
    std::string email, password;
    std::cout << "Email: ";
    std::getline(std::cin, email);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    AuthService authService(httpClient);
    std::string response = authService.login(email, password);
    std::cout << "\nResponse from server: " << response << "\n";

    try {
        auto res = json::parse(response);
        if (res.value("success", false) == true) {
            std::cout << "Login successful!\n";
            
            if (res.contains("user_id")) {
                if (res["user_id"].is_number()) {
                    userSession.setUserId(res.value("user_id", 0));
                } 
            }
            
            if (res.contains("email")) {
                userSession.setEmail(res.value("email", email));
            } else if (res.contains("username")) {
                userSession.setUsername(res.value("username", ""));
            }

            std::string userRole = "user"; 
            if (res.contains("role")) {
                if (res["role"].is_string()) {
                    userRole = res["role"].get<std::string>();
                } else if (res["role"].is_number()) {
                    int roleNum = res["role"].get<int>();
                    userRole = (roleNum == 1) ? "admin" : "user";
                }
            }
            userSession.setRole(userRole);

            if (userRole == "admin") {
                AdminMenu(httpClient, userSession).display();
            } else {
                UserMenu(httpClient, userSession).display();
            }
        } else {
            std::cout << "Login failed: " << res.value("message", "Unknown error") << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Invalid response from server: " << e.what() << "\n";
        std::cout << "Raw response: " << response << "\n"; 
    }
}

void HomeMenu::displaySignup() {
    std::string username, email, password;

    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::cout << "Email: ";
    std::getline(std::cin, email);
    if (!Validator::isValidEmail(email)) {
        std::cout << "Invalid email format.\n";
        return;
    }

    std::cout << "Password: ";
    std::getline(std::cin, password);
    if (!Validator::isValidPassword(password)) {
        std::cout << "Password must be at least 6 characters.\n";
        return;
    }

    AuthService authService(httpClient);
    std::string response = authService.signup(username, email, password);

    try {
        auto res = json::parse(response);
        if (res.contains("success") && res["success"] == true) {
            std::cout << "Signup successful! Please login.\n";
        } else {
            std::cout << "Signup failed: " << res.value("message", "Unknown error") << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Invalid response from server: " << e.what() << "\n";
    }
}

void HomeMenu::exitApplication() {
    std::cout << "Exiting application. Goodbye!\n";
    exit(0);
}
