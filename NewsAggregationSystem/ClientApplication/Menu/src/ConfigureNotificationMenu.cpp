#include "../inc/ConfigureNotificationMenu.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ConfigureNotificationMenu::ConfigureNotificationMenu(Client& c, Session& s) : client(c), session(s) {}

void ConfigureNotificationMenu::display() {
    while (true) {
        std::cout << "\nC O N F I G U R E - N O T I F I C A T I O N S\n"
                  << "1. Business - Enabled\n"
                  << "2. Entertainment - Enabled\n"
                  << "3. Sports - Disabled\n"
                  << "4. Technology - Disabled\n"
                  << "5. Keywords - Enabled\n"
                  << "6. Back\n"
                  << "7. Logout\n"
                  << "Enter your option: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice >= 1 && choice <= 4) {
            toggleCategory(choice);
        } else if (choice == 5) {
            configureKeywords();
        } else if (choice == 6) {
            return;
        } else if (choice == 7) {
            exit(0);
        } else {
            std::cout << "Invalid option.\n";
        }
    }
}

void ConfigureNotificationMenu::toggleCategory(int categoryId) {
    std::string status;
    std::cout << "Enable or Disable this category? (enable/disable): ";
    std::getline(std::cin, status);

    json body = {
        {"user_id", session.getUserId()},
        {"category_id", categoryId},
        {"enabled", status == "enable"}
    };

    std::string res = client.post("/notifications/configure/category", body.dump());
    std::cout << "Server: " << res << "\n";
}

void ConfigureNotificationMenu::configureKeywords() {
    std::string keywords;
    std::cout << "Enter keywords separated by commas (e.g., Tesla, Election, Crypto): ";
    std::getline(std::cin, keywords);

    json body = {
        {"user_id", session.getUserId()},
        {"keywords", keywords}
    };

    std::string res = client.post("/notifications/configure/keywords", body.dump());
    std::cout << "Server: " << res << "\n";
}
