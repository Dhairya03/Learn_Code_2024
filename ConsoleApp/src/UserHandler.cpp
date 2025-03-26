#include "UserHandler.h"
#include <iostream>

bool UserHandler::login() {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;
    
    std::string response = HttpClient::sendRequest("http://localhost:8080/login", "POST", "{\"username\": \"" + username + "\", \"password\": \"" + password + "\"}");
    
    if (response == "Login successful") {
        std::cout << "Login successful!\n";
        return true;
    }
    std::cout << "Login failed.\n";
    return false;
}

void UserHandler::signup() {
    std::string username, password;
    std::cout << "Enter new username: ";
    std::cin >> username;
    std::cout << "Enter new password: ";
    std::cin >> password;
    
    std::string response = HttpClient::sendRequest("http://localhost:8080/signup", "POST", "{\"username\": \"" + username + "\", \"password\": \"" + password + "\"}");
    std::cout << response << "\n";
}