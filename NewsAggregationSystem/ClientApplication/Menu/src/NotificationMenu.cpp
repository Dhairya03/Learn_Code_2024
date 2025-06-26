#include "../inc/NotificationMenu.h"
#include "../inc/ConfigureNotificationMenu.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

NotificationMenu::NotificationMenu(Client& c, Session& s) : client(c), session(s) {}

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
    std::string endpoint = "/notifications?user_id=" + std::to_string(session.getUserId());
    std::string response = client.get(endpoint);

    try {
        auto notifications = json::parse(response);
        std::cout << "\nYou have " << notifications.size() << " notifications:\n";
        for (const auto& n : notifications) {
            std::cout << "- " << n["message"] << " (" << n["timestamp"] << ")\n";
        }
    } catch (...) {
        std::cout << "Failed to parse notifications.\n";
    }
}

void NotificationMenu::configureNotifications() {
    ConfigureNotificationMenu(client, session).display();
}
