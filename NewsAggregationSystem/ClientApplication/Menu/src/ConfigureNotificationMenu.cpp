#include "../inc/ConfigureNotificationMenu.h"
#include "../../Services/inc/NotificationService.h"
#include "../../Services/inc/CategoryService.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ConfigureNotificationMenu::ConfigureNotificationMenu(Client& httpClient, Session& userSession)
    : httpClient(httpClient), userSession(userSession), notificationService(httpClient), categoryService(httpClient) {}

void ConfigureNotificationMenu::display() {
    while (true) {
        auto settings = notificationService.getUserNotificationSettings(userSession.getUserId());
        auto categories = categoryService.getAllCategories();
        
        std::cout << "\nC O N F I G U R E - N O T I F I C A T I O N S\n";
        
        for (size_t i = 0; i < categories.size(); ++i) {
            std::string categoryName = categories[i];
            bool enabled = settings.value(categoryName + "_enabled", false);
            std::cout << (i + 1) << ". " << categoryName << " - " << (enabled ? "Enabled" : "Disabled") << "\n";
        }
        
        std::cout << (categories.size() + 1) << ". Keywords - " << (settings.value("keywords_enabled", false) ? "Enabled" : "Disabled") << "\n";
        
        std::cout << (categories.size() + 2) << ". Back\n"
                  << (categories.size() + 3) << ". Logout\n"
                  << "Enter your option: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        int categoriesSize = categories.size();
        
        if (choice >= 1 && choice <= static_cast<int>(categoriesSize)) {
            toggleCategorySetting(choice - 1, categories);
        } else if (choice == categoriesSize + 1) {
            configureNotificationKeywords();
        } else if (choice == categoriesSize + 2) {
            return;
        } else if (choice == categoriesSize + 3) {
            exit(0);
        } else {
            std::cout << "Invalid option.\n";
        }
    }
}

void ConfigureNotificationMenu::toggleCategorySetting(int categoryIndex, const std::vector<std::string>& categories) {
    if (categoryIndex < 0 || categoryIndex >= static_cast<int>(categories.size())) {
        std::cout << "Invalid category index.\n";
        return;
    }
    
    std::string categoryName = categories[categoryIndex];
    std::string status;
    std::cout << "Enable or Disable " << categoryName << "? (enable/disable): ";
    std::getline(std::cin, status);

    bool enabled = (status == "enable");
    bool success = notificationService.updateCategorySettings(userSession.getUserId(), categoryName, enabled);
    
    if (success) {
        std::cout << categoryName << " settings updated successfully.\n";
    } else {
        std::cout << "Failed to update " << categoryName << " settings.\n";
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
