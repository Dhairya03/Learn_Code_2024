#include "../inc/SearchMenu.h"
#include "../../Services/inc/SavedArticleService.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SearchMenu::SearchMenu(Client& c, Session& s) : client(c), session(s) {}

void SearchMenu::display() {
    std::cout << "\nS E A R C H\n";
    performSearch();
}

void SearchMenu::performSearch() {
    std::string query, start, end;
    std::cout << "Enter search keyword: ";
    std::getline(std::cin, query);

    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::getline(std::cin, start);
    std::cout << "Enter end date (YYYY-MM-DD): ";
    std::getline(std::cin, end);

    std::string endpoint = "/news/search?query=" + query +
                           "&start=" + start + "&end=" + end + "&sort=likes";

    std::string response = client.get(endpoint);

    try {
        auto articles = json::parse(response);
        std::cout << "Results for \"" << query << "\":\n";

        for (const auto& a : articles) {
            std::cout << "Article ID: " << a["id"]
                      << "\nTitle: " << a["title"]
                      << "\nSource: " << a["source"]
                      << "\nURL: " << a["url"]
                      << "\nCategory: " << a["category"]
                      << "\n\n";
        }

        std::cout << "1. Back\n2. Logout\n3. Save Article\n>> ";
        int action;
        std::cin >> action;
        std::cin.ignore();

        if (action == 3) {
            int id;
            std::cout << "Enter Article ID to save: ";
            std::cin >> id;
            std::cin.ignore();
            SavedArticleService(client, session).saveArticle(id);
        } else if (action == 2) {
            exit(0);
        }

    } catch (...) {
        std::cout << "Invalid search response.\n";
    }
}
