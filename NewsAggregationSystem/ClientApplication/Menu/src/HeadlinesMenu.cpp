#include "../inc/HeadlinesMenu.h"
#include "../../Services/inc/ArticleService.h"
#include "../../Services/inc/SavedArticleService.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

HeadlinesMenu::HeadlinesMenu(Client& c, Session& s) : client(c), session(s) {}

void HeadlinesMenu::display() {
    std::cout << "\nHeadlines:\n"
              << "1. Today\n"
              << "2. Date range\n"
              << "3. Logout\n>> ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
        case 1: viewTodayHeadlines(); break;
        case 2: viewDateRangeHeadlines(); break;
        case 3: exit(0);
        default: std::cout << "Invalid option.\n";
    }
}

void HeadlinesMenu::viewTodayHeadlines() {
    std::string today = ArticleService::getTodayDate();
    fetchAndDisplayArticles(today, today, "all");
}

void HeadlinesMenu::viewDateRangeHeadlines() {
    std::string start, end;
    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::getline(std::cin, start);
    std::cout << "Enter end date (YYYY-MM-DD): ";
    std::getline(std::cin, end);

    std::string category = selectCategory();
    fetchAndDisplayArticles(start, end, category);
}

std::string HeadlinesMenu::selectCategory() {
    std::cout << "\nPlease choose the options below for Headlines:\n"
              << "1. All\n2. Business\n3. Entertainment\n4. Sports\n5. Technology\n6. General\n>> ";
    int opt;
    std::cin >> opt;
    std::cin.ignore();

    switch (opt) {
        case 1: return "all";
        case 2: return "business";
        case 3: return "entertainment";
        case 4: return "sports";
        case 5: return "technology";
        case 6: return "general";
        default: return "all";
    }
}

void HeadlinesMenu::fetchAndDisplayArticles(const std::string& start, const std::string& end, const std::string& category) {
    auto articles = ArticleService(client).getArticles(start, end, category);

    std::cout << "\nH E A D L I N E S\n";
    for (const auto& a : articles) {
        std::cout << "Article ID: " << a["id"] << "\nTitle: " << a["title"]
                  << "\nSource: " << a["source"] << "\nURL: " << a["url"]
                  << "\nCategory: " << a["category"] << "\n\n";
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
}
