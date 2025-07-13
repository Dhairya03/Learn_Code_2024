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
    
    json requestData;
    requestData["id"] = id;
    requestData["api_key"] = newApiKey;
    
    std::string resStr = httpClient.put("/admin/servers/update", requestData.dump());
    try {
        auto response = json::parse(resStr);
        if (response["success"]) {
            std::cout << "API key updated successfully for server ID " << id << ".\n";
        } else {
            std::cout << "Error: " << response.value("message", "Failed to update API key") << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Failed to update API key: " << e.what() << "\n";
    }
}

void AdminMenu::addNewsCategory() {
    std::string categoryName;
    std::cout << "Enter new category name: ";
    std::getline(std::cin, categoryName);
    
    json requestData;
    requestData["name"] = categoryName;
    
    std::string resStr = httpClient.post("/admin/categories/add", requestData.dump());
    try {
        auto response = json::parse(resStr);
        if (response["success"]) {
            std::cout << "Category '" << categoryName << "' added successfully.\n";
        } else {
            std::cout << "Error: " << response.value("message", "Failed to add category") << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Failed to add category: " << e.what() << "\n";
    }
}

void AdminMenu::displayReportedArticles() {
    std::string resStr = httpClient.get("/admin/articles/reported");
    try {
        auto response = json::parse(resStr);
        if (!response["success"]) {
            std::cout << "Error: " << response.value("message", "Unknown error") << "\n";
            return;
        }
        auto data = response["data"];
        if (data.empty()) {
            std::cout << "No reported articles found.\n";
            return;
        }
        std::cout << "\nReported Articles:\n";
        std::cout << "+----+--------------------------------+------------------+----------+\n";
        std::cout << "| ID | Title                          | Source           | Reports  |\n";
        std::cout << "+----+--------------------------------+------------------+----------+\n";
        for (const auto& article : data) {
            std::cout << "| "
                      << std::setw(2) << article["id"] << " | "
                      << std::setw(30) << article["title"].get<std::string>().substr(0,30) << " | "
                      << std::setw(16) << article["source"].get<std::string>().substr(0,16) << " | "
                      << std::setw(8) << article["report_count"] << " |\n";
        }
        std::cout << "+----+--------------------------------+------------------+----------+\n";
    } catch (const std::exception& e) {
        std::cout << "Failed to parse reported articles: " << e.what() << "\n";
    }
}

void AdminMenu::toggleCategoryVisibility() {
    int categoryId;
    std::cout << "Enter category ID to toggle visibility: ";
    std::cin >> categoryId;
    std::cin.ignore();
    
    json requestData;
    requestData["category_id"] = categoryId;
    
    std::string resStr = httpClient.put("/admin/categories/toggle-visibility", requestData.dump());
    try {
        auto response = json::parse(resStr);
        if (response["success"]) {
            std::cout << "Category visibility toggled successfully for ID " << categoryId << ".\n";
        } else {
            std::cout << "Error: " << response.value("message", "Failed to toggle category visibility") << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Failed to toggle category visibility: " << e.what() << "\n";
    }
}

void AdminMenu::displayNotifications() {
    std::string resStr = httpClient.get("/admin/notifications");
    try {
        auto response = json::parse(resStr);
        if (!response["success"]) {
            std::cout << "Error: " << response.value("message", "Unknown error") << "\n";
            return;
        }
        auto data = response["data"];
        if (data.empty()) {
            std::cout << "No notifications found.\n";
            return;
        }
        std::cout << "\nNotifications:\n";
        std::cout << "+----+------------------+--------------------------------+------------------+\n";
        std::cout << "| ID | Type             | Message                        | Timestamp        |\n";
        std::cout << "+----+------------------+--------------------------------+------------------+\n";
        for (const auto& notification : data) {
            std::cout << "| "
                      << std::setw(2) << notification["id"] << " | "
                      << std::setw(16) << notification["type"].get<std::string>().substr(0,16) << " | "
                      << std::setw(30) << notification["message"].get<std::string>().substr(0,30) << " | "
                      << std::setw(16) << notification["timestamp"].get<std::string>().substr(0,16) << " |\n";
        }
        std::cout << "+----+------------------+--------------------------------+------------------+\n";
    } catch (const std::exception& e) {
        std::cout << "Failed to parse notifications: " << e.what() << "\n";
    }
}
