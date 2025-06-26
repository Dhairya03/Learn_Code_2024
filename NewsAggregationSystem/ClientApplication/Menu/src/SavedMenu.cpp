#include "../inc/SavedMenu.h"
#include "../../Services/inc/SavedArticleService.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SavedMenu::SavedMenu(Client& c, Session& s) : client(c), session(s) {}

void SavedMenu::display() {
    while (true) {
        std::cout << "\nS A V E D  A R T I C L E S\n";
        showSavedArticles();

        std::cout << "\nOptions:\n"
                  << "1. Back\n"
                  << "2. Logout\n"
                  << "3. Delete Article\n>> ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) return;
        if (choice == 2) exit(0);
        if (choice == 3) deleteSavedArticle();
        else std::cout << "Invalid choice.\n";
    }
}

void SavedMenu::showSavedArticles() {
    std::string res = client.get("/user/articles");

    try {
        auto articles = json::parse(res);
        for (const auto& a : articles) {
            std::cout << "\nArticle ID: " << a["id"]
                      << "\nTitle: " << a["title"]
                      << "\nDescription: " << a["description"]
                      << "\nSource: " << a["source"]
                      << "\nURL: " << a["url"]
                      << "\nCategory: " << a["category"]
                      << "\n";
        }
    } catch (...) {
        std::cout << "Failed to load saved articles.\n";
    }
}

void SavedMenu::deleteSavedArticle() {
    int id;
    std::cout << "Enter article ID to delete: ";
    std::cin >> id;
    std::cin.ignore();

    SavedArticleService(client, session).deleteArticle(id);
}
