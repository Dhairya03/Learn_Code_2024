#include "../inc/NotificationMenu.h"
#include "../inc/ConfigureNotificationMenu.h"
#include "../../Services/inc/NotificationService.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <iomanip>

using json = nlohmann::json;

NotificationMenu::NotificationMenu(Client& httpClient, Session& userSession)
    : httpClient(httpClient), userSession(userSession), notificationService(httpClient) {}

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
            case 1: displayNotificationsTable(); break;
            case 2: displayConfigureNotificationsMenu(); break;
            case 3: return;
            case 4: exit(0);
            default: std::cout << "Invalid option.\n";
        }
    }
}

void NotificationMenu::displayNotificationsTable() {
    auto notifications = notificationService.getNotifications(userSession.getUserId());
    if (notifications.empty()) {
        std::cout << "\nNo notifications found.\n";
        return;
    }
    std::cout << "\n+----------------------------------------------------------+--------------+---------------------+-------------------+\n";
    std::cout << "| Message                                                  | Type         | Timestamp           | Category          |\n";
    std::cout << "+----------------------------------------------------------+--------------+---------------------+-------------------+\n";
    for (const auto& notification : notifications) {
        std::cout << "| " << std::setw(58) << notification["message"].get<std::string>().substr(0,58) << " | "
                  << std::setw(12) << notification["type"].get<std::string>().substr(0,12) << " | "
                  << std::setw(19) << notification["timestamp"].get<std::string>().substr(0,19) << " | ";
        if (notification.contains("category_name") && !notification["category_name"].is_null()) {
            std::cout << std::setw(17) << notification["category_name"].get<std::string>().substr(0,17);
        } else {
            std::cout << std::setw(17) << "-";
        }
        std::cout << " |\n";
    }
    std::cout << "+----------------------------------------------------------+--------------+---------------------+-------------------+\n";
}

void NotificationMenu::displayConfigureNotificationsMenu() {
    ConfigureNotificationMenu(httpClient, userSession).display();
}
