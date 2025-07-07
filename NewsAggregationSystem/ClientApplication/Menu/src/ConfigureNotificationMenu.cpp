#include "../inc/ConfigureNotificationMenu.h"
#include "../../Services/inc/NotificationService.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ConfigureNotificationMenu::ConfigureNotificationMenu(Client& httpClient, Session& userSession)
    : httpClient(httpClient), userSession(userSession), notificationService(httpClient) {}

void ConfigureNotificationMenu::display() {
    while (true) {
        // Get current settings
        auto settings = notificationService.getUserNotificationSettings(userSession.getUserId());
        
        std::cout << "\nC O N F I G U R E - N O T I F I C A T I O N S\n"
                  << "1. Business - " << (settings.value("business_enabled", false) ? "Enabled" : "Disabled") << "\n"
                  << "2. Entertainment - " << (settings.value("entertainment_enabled", false) ? "Enabled" : "Disabled") << "\n"
                  << "3. Sports - " << (settings.value("sports_enabled", false) ? "Enabled" : "Disabled") << "\n"
                  << "4. Technology - " << (settings.value("technology_enabled", false) ? "Enabled" : "Disabled") << "\n"
                  << "5. Keywords - " << (settings.value("keywords_enabled", false) ? "Enabled" : "Disabled") << "\n"
                  << "6. Back\n"
                  << "7. Logout\n"
                  << "Enter your option: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice >= 1 && choice <= 4) {
            toggleCategorySetting(choice);
        } else if (choice == 5) {
            configureNotificationKeywords();
        } else if (choice == 6) {
            return;
        } else if (choice == 7) {
            exit(0);
        } else {
            std::cout << "Invalid option.\n";
        }
    }
}

void ConfigureNotificationMenu::toggleCategorySetting(int categoryId) {
    std::string status;
    std::cout << "Enable or Disable this category? (enable/disable): ";
    std::getline(std::cin, status);

    bool enabled = (status == "enable");
    bool success = notificationService.updateCategorySettings(userSession.getUserId(), categoryId, enabled);
    
    if (success) {
        std::cout << "Category settings updated successfully.\n";
    } else {
        std::cout << "Failed to update category settings.\n";
    }
}

void ConfigureNotificationMenu::configureNotificationKeywords() {
    std::string keywords;
    std::cout << "Enter keywords separated by commas (e.g., Tesla, Election, Crypto): ";
    std::getline(std::cin, keywords);

    bool success = notificationService.updateKeywords(userSession.getUserId(), keywords);
    
    if (success) {
        std::cout << "Keywords updated successfully.\n";
    } else {
        std::cout << "Failed to update keywords.\n";
    }
}
