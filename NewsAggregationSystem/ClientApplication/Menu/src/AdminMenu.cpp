#include "../inc/AdminMenu.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <iomanip>

using json = nlohmann::json;

AdminMenu::AdminMenu(Client& httpClient, Session& userSession) : httpClient(httpClient), userSession(userSession) {}

void AdminMenu::display() {
    while (true) {
        std::cout << "\nAdmin Menu:\n"
                  << "1. View the list of external servers and status\n"
                  << "2. View the external server's details\n"
                  << "3. Update/Edit the external server's details\n"
                  << "4. Add new News Category\n"
                  << "5. View Reported Articles\n"
                  << "6. Hide/Unhide Category\n"
                  << "7. View Notifications\n"
                  << "8. Logout\n>> ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: displayServerStatus(); break;
            case 2: displayServerDetails(); break;
            case 3: updateServerApiKey(); break;
            case 4: addNewsCategory(); break;
            case 5: displayReportedArticles(); break;
            case 6: toggleCategoryVisibility(); break;
            case 7: displayNotifications(); break;
            case 8: return;
            default: std::cout << "Invalid option.\n";
        }
    }
}

void AdminMenu::displayServerStatus() {
    std::string resStr = httpClient.get("/admin/servers/status");
    try {
        auto response = json::parse(resStr);
        if (!response["success"]) {
            std::cout << "Error: " << response.value("message", "Unknown error") << "\n";
            return;
        }
        auto data = response["data"];
        if (data.empty()) {
            std::cout << "No external servers found.\n";
            return;
        }
        std::cout << "\n+----+-------------------+---------------------+--------------+\n";
        std::cout << "| ID | Name              | Last Accessed       | Status       |\n";
        std::cout << "+----+-------------------+---------------------+--------------+\n";
        for (const auto& server : data) {
            std::cout << "| "
                      << std::setw(2) << server["id"] << " | "
                      << std::setw(17) << server["name"].get<std::string>().substr(0,17) << " | "
                      << std::setw(19) << server["last_accessed"].get<std::string>().substr(0,19) << " | "
                      << std::setw(12) << server["status"].get<std::string>().substr(0,12) << " |\n";
        }
        std::cout << "+----+-------------------+---------------------+--------------+\n";
    } catch (const std::exception& e) {
        std::cout << "Failed to parse server status: " << e.what() << "\n";
    }
}

void AdminMenu::displayServerDetails() {
    std::string resStr = httpClient.get("/admin/servers/details");
    try {
        auto response = json::parse(resStr);
        if (!response["success"]) {
            std::cout << "Error: " << response.value("message", "Unknown error") << "\n";
            return;
        }
        auto data = response["data"];
        if (data.empty()) {
            std::cout << "No external server details found.\n";
            return;
        }
        std::cout << "\n+----+-------------------+------------------------------------------+\n";
        std::cout << "| ID | Name              | API Key                                   |\n";
        std::cout << "+----+-------------------+------------------------------------------+\n";
        for (const auto& server : data) {
            std::cout << "| "
                      << std::setw(2) << server["id"] << " | "
                      << std::setw(17) << server["name"].get<std::string>().substr(0,17) << " | "
                      << std::setw(40) << server["api_key"].get<std::string>().substr(0,40) << " |\n";
        }
        std::cout << "+----+-------------------+------------------------------------------+\n";
    } catch (const std::exception& e) {
        std::cout << "Failed to parse server details: " << e.what() << "\n";
    }
}

void AdminMenu::updateServerApiKey() {
    int id;
    std::string newApiKey;
    std::cout << "Enter external server ID: ";
    std::cin >> id;
    std::cin.ignore();
    std::cout << "Enter the updated API key: ";
    std::getline(std::cin, newApiKey);
    // Implement API call to update the API key here
    std::cout << "API key updated for server ID " << id << ".\n";
}

void AdminMenu::addNewsCategory() {
    std::string categoryName;
    std::cout << "Enter new category name: ";
    std::getline(std::cin, categoryName);
    // Implement API call to add category here
    std::cout << "Category '" << categoryName << "' added.\n";
}

void AdminMenu::displayReportedArticles() {
    // Implement logic to display reported articles
    std::cout << "Displaying reported articles (not yet implemented).\n";
}

void AdminMenu::toggleCategoryVisibility() {
    // Implement logic to hide/unhide category
    std::cout << "Toggling category visibility (not yet implemented).\n";
}

void AdminMenu::displayNotifications() {
    // Implement logic to display notifications
    std::cout << "Displaying notifications (not yet implemented).\n";
}
