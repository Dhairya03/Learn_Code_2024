#include "../inc/UserMenu.h"
#include "../inc/HeadlinesMenu.h"
#include "../inc/SavedMenu.h"
#include "../inc/SearchMenu.h"
#include "../inc/NotificationMenu.h"
#include "../inc/PersonalizedMenu.h"
#include <iostream>
#include <ctime>

UserMenu::UserMenu(Client& httpClient, Session& userSession) : httpClient(httpClient), userSession(userSession) {}

void UserMenu::display() {
    while (true) {
        time_t now = time(nullptr);
        std::cout << "\nWelcome to the News Application, " << userSession.getUsername()
                  << "! Date: " << std::ctime(&now)
                  << "Please choose the options below:\n"
                  << "1. Headlines\n"
                  << "2. Saved Articles\n"
                  << "3. Search\n"
                  << "4. Notifications\n"
                  << "5. Personalized Content\n"
                  << "6. Logout\n>> ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: displayHeadlines(); break;
            case 2: displaySavedArticles(); break;
            case 3: displaySearch(); break;
            case 4: displayNotifications(); break;
            case 5: displayPersonalizedContent(); break;
            case 6: return;
            default: std::cout << "Invalid option.\n";
        }
    }
}

void UserMenu::displayHeadlines() {
    HeadlinesMenu(httpClient, userSession).display();
}

void UserMenu::displaySavedArticles() {
    SavedMenu(httpClient, userSession).display();
}

void UserMenu::displaySearch() {
    SearchMenu(httpClient, userSession).display();
}

void UserMenu::displayNotifications() {
    NotificationMenu(httpClient, userSession).display();
}

void UserMenu::displayPersonalizedContent() {
    PersonalizedMenu(httpClient, userSession).display();
}
