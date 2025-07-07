#include "../inc/NotificationMenu.h"
#include "../inc/ConfigureNotificationMenu.h"
#include "../../Services/inc/NotificationService.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

NotificationMenu::NotificationMenu(Client& c, Session& s) : client(c), session(s), notificationService(c) {}

void NotificationMenu::display() {
    while (true) {
        std::cout << "\nN O T I F I C A T I O N S\n"
                  << "1. View Notifications\n"
                  << "2. Configure Notifications\n"
                  << "3. Back\n"
                  << "4. Logout\n>> ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: viewNotifications(); break;
            case 2: configureNotifications(); break;
            case 3: return;
            case 4: exit(0);
            default: std::cout << "Invalid option.\n";
        }
    }
}

void NotificationMenu::viewNotifications() {
    auto notifications = notificationService.getNotifications(session.getUserId());
    
    if (notifications.empty()) {
        std::cout << "\nNo notifications found.\n";
        return;
    }
    
    std::cout << "\nYou have " << notifications.size() << " notifications:\n";
    for (const auto& notification : notifications) {
        std::cout << "- " << notification["message"] << " (" << notification["timestamp"] << ")\n";
        std::cout << "  Type: " << notification["type"] << "\n";
        if (notification.contains("category_name") && !notification["category_name"].is_null()) {
            std::cout << "  Category: " << notification["category_name"] << "\n";
        }
        std::cout << "\n";
    }
}

void NotificationMenu::configureNotifications() {
    ConfigureNotificationMenu(client, session).display();
}
