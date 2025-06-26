#include "../inc/AdminMenu.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

AdminMenu::AdminMenu(Client& c, Session& s) : client(c), session(s) {}

void AdminMenu::display() {
    while (true) {
        std::cout << "\nAdmin Menu:\n"
                  << "1. View the list of external servers and status\n"
                  << "2. View the external server’s details\n"
                  << "3. Update/Edit the external server’s details\n"
                  << "4. Add new News Category\n"
                  << "5. Logout\n>> ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: viewServerStatus(); break;
            case 2: viewServerDetails(); break;
            case 3: updateServerDetails(); break;
            case 4: addCategory(); break;
            case 5: return;
            default: std::cout << "Invalid option.\n";
        }
    }
}

void AdminMenu::viewServerStatus() {
    std::string resStr = client.get("/admin/servers/status");
    std::cout << "\nExternal Servers and Status:\n" << resStr << "\n";
}

void AdminMenu::viewServerDetails() {
    std::string resStr = client.get("/admin/servers/details");
    std::cout << "\nExternal Server Details:\n" << resStr << "\n";
}

void AdminMenu::updateServerDetails() {
    int id;
    std::string newKey;
    std::cout << "Enter the external server ID: ";
    std::cin >> id;
    std::cin.ignore();
    std::cout << "Enter the updated API key: ";
    std::getline(std::cin, newKey);

    json body = {{"id", id}, {"api_key", newKey}};
    std::string res = client.post("/admin/servers/update", body.dump());
    std::cout << "Response: " << res << "\n";
}

void AdminMenu::addCategory() {
    std::string name;
    std::cout << "Enter new category name: ";
    std::getline(std::cin, name);

    json body = {{"name", name}};
    std::string res = client.post("/admin/categories/add", body.dump());
    std::cout << "Response: " << res << "\n";
}
