#include "../inc/UserMenu.h"
#include "../inc/HeadlinesMenu.h"
#include "../inc/SavedMenu.h"
#include "../inc/SearchMenu.h"
#include "../inc/NotificationMenu.h"
#include <iostream>
#include <ctime>

UserMenu::UserMenu(Client& c, Session& s) : client(c), session(s) {}

void UserMenu::display() {
    while (true) {
        time_t now = time(nullptr);
        std::cout << "\nWelcome to the News Application, " << session.getUsername()
                  << "! Date: " << std::ctime(&now)
                  << "Please choose the options below:\n"
                  << "1. Headlines\n"
                  << "2. Saved Articles\n"
                  << "3. Search\n"
                  << "4. Notifications\n"
                  << "5. Logout\n>> ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: showHeadlines(); break;
            case 2: showSavedArticles(); break;
            case 3: showSearch(); break;
            case 4: showNotifications(); break;
            case 5: return;
            default: std::cout << "Invalid option.\n";
        }
    }
}

void UserMenu::showHeadlines() {
    HeadlinesMenu(client, session).display();
}

void UserMenu::showSavedArticles() {
    SavedMenu(client, session).display();
}

void UserMenu::showSearch() {
    SearchMenu(client, session).display();
}

void UserMenu::showNotifications() {
    NotificationMenu(client, session).display();
}
