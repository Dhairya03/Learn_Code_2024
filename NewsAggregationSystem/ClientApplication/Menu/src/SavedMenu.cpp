#include "../inc/SavedMenu.h"
#include "../../Services/inc/SavedArticleService.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <iomanip>

using json = nlohmann::json;

SavedMenu::SavedMenu(Client& httpClient, Session& userSession) : httpClient(httpClient), userSession(userSession) {}

void SavedMenu::display() {
    while (true) {
        std::cout << "\nS A V E D  A R T I C L E S\n";
        displaySavedArticlesTable();

        std::cout << "\nOptions:\n"
                  << "1. Back\n"
                  << "2. Logout\n"
                  << "3. Delete Article\n>> ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) return;
        if (choice == 2) exit(0);
        if (choice == 3) removeSavedArticle();
        else std::cout << "Invalid choice.\n";
    }
}

void SavedMenu::displaySavedArticlesTable() {
    std::string res = httpClient.get("/user/articles?user_id=" + std::to_string(userSession.getUserId()));

    try {
        auto parsed = json::parse(res);
        std::vector<json> articles;
        if (parsed.is_array()) {
            articles = parsed;
        } else if (parsed.is_object() && parsed.contains("data")) {
            articles = parsed["data"].get<std::vector<json>>();
        }
        if (articles.empty()) {
            std::cout << "No saved articles found.\n";
            return;
        }
        std::cout << "+-----+----------------------------------------------------------+-------------------+--------------+------------------------------------------+\n";
        std::cout << "| ID  | Title                                                    | Source            | Category     | URL                                      |\n";
        std::cout << "+-----+----------------------------------------------------------+-------------------+--------------+------------------------------------------+\n";
        for (const auto& a : articles) {
            std::cout << "| " << std::setw(3) << a["id"] << " | "
                      << std::setw(58) << a["title"].get<std::string>().substr(0,58) << " | "
                      << std::setw(17) << a["source"].get<std::string>().substr(0,17) << " | "
                      << std::setw(12) << (a.contains("category") ? a["category"].get<std::string>().substr(0,12) : "-") << " | "
                      << std::setw(40) << a["url"].get<std::string>().substr(0,40) << " |\n";
        }
        std::cout << "+-----+----------------------------------------------------------+-------------------+--------------+------------------------------------------+\n";
    } catch (...) {
        std::cout << "Failed to load saved articles.\n";
    }
}

void SavedMenu::removeSavedArticle() {
    int articleId;
    std::cout << "Enter Article ID to delete: ";
    std::cin >> articleId;
    std::cin.ignore();
    SavedArticleService(httpClient, userSession).deleteArticle(articleId);
    std::cout << "Article deleted.\n";
}
