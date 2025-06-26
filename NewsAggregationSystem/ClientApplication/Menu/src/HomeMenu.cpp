#include "../../Menu/inc/HomeMenu.h"
#include "../../Menu/inc/AdminMenu.h"
#include "../../Menu/inc/UserMenu.h"
#include "../../Services/inc/AuthService.h"
#include "../../Utils/inc/Validator.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

HomeMenu::HomeMenu(Client& c, Session& s) : client(c), session(s) {}

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
            case 1: login(); break;
            case 2: signup(); break;
            case 3: exit(0);
            default: std::cout << "Invalid option.\n";
        }
    }
}

void HomeMenu::login() {
    std::string email, password;
    std::cout << "Email: ";
    std::getline(std::cin, email);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    AuthService authService(client);
    std::string response = authService.login(email, password);
    std::cout << "\nResponse from server: " << response << "\n";

    try {
        auto res = json::parse(response);
        if (res.value("success", false) == true) {
            std::cout << "Login successful!\n";
            
            if (res.contains("user_id")) {
                if (res["user_id"].is_number()) {
                    session.setUserId(res.value("user_id", 0));
                } 
            }
            
            if (res.contains("email")) {
                session.setEmail(res.value("email", email));
            } else if (res.contains("username")) {
                session.setUsername(res.value("username", ""));
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
            session.setRole(userRole);

            if (userRole == "admin") {
                AdminMenu(client, session).display();
            } else {
                UserMenu(client, session).display();
            }
        
        } else {
            std::cout << "Login failed: " << res.value("message", "Unknown error") << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Invalid response from server: " << e.what() << "\n";
        std::cout << "Raw response: " << response << "\n"; 
    }
}

void HomeMenu::signup() {
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

    AuthService authService(client);
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
