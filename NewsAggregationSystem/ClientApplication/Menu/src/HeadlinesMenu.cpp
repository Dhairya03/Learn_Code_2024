#include "../inc/HeadlinesMenu.h"
#include "../../Services/inc/ArticleService.h"
#include "../../Services/inc/SavedArticleService.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <iomanip>

using json = nlohmann::json;

HeadlinesMenu::HeadlinesMenu(Client& httpClient, Session& userSession) : httpClient(httpClient), userSession(userSession) {}

void HeadlinesMenu::display() {
    std::cout << "\nHeadlines:\n"
              << "1. Today\n"
              << "2. Date range\n"
              << "3. Logout\n>> ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
        case 1: displayTodayHeadlines(); break;
        case 2: displayDateRangeHeadlines(); break;
        case 3: exit(0);
        default: std::cout << "Invalid option.\n";
    }
}

void HeadlinesMenu::displayTodayHeadlines() {
    std::string today = ArticleService::getTodayDate();
    fetchAndDisplayArticlesTable(today, today, "all");
}

void HeadlinesMenu::displayDateRangeHeadlines() {
    std::string start, end;
    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::getline(std::cin, start);
    std::cout << "Enter end date (YYYY-MM-DD): ";
    std::getline(std::cin, end);

    std::string category = selectHeadlineCategory();
    fetchAndDisplayArticlesTable(start, end, category);
}

std::string HeadlinesMenu::selectHeadlineCategory() {
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

void HeadlinesMenu::fetchAndDisplayArticlesTable(const std::string& start, const std::string& end, const std::string& category) {
    auto articles = ArticleService(httpClient).getArticles(start, end, category);

    if (articles.empty()) {
        std::cout << "\nNo articles found.\n";
    } else {
        std::cout << "\n+-----+----------------------------------------------------------+-------------------+--------------+------------------------------------------+\n";
        std::cout << "| ID  | Title                                                    | Source            | Category     | URL                                      |\n";
        std::cout << "+-----+----------------------------------------------------------+-------------------+--------------+------------------------------------------+\n";
        for (const auto& a : articles) {
            std::cout << "| " << std::setw(3) << a["id"] << " | "
                      << std::setw(58) << a["title"].get<std::string>().substr(0,58) << " | "
                      << std::setw(17) << a["source"].get<std::string>().substr(0,17) << " | "
                      << std::setw(12) << a["category"].get<std::string>().substr(0,12) << " | "
                      << std::setw(40) << a["url"].get<std::string>().substr(0,40) << " |\n";
        }
        std::cout << "+-----+----------------------------------------------------------+-------------------+--------------+------------------------------------------+\n";
    }
    std::cout << "1. Back\n2. Logout\n3. Save Article\n4. Like Article\n5. Dislike Article\n6. Report Article\n>> ";
    int action;
    std::cin >> action;
    std::cin.ignore();

    if (action == 3) {
        int id;
        std::cout << "Enter Article ID to save: ";
        std::cin >> id;
        std::cin.ignore();
        SavedArticleService(httpClient, userSession).saveArticle(id);
    } else if (action == 4) {
        int id;
        std::cout << "Enter Article ID to like: ";
        std::cin >> id;
        std::cin.ignore();
        std::cout << "Liked Article ID: " << id << " (stub, backend not implemented yet)\n";
    } else if (action == 5) {
        int id;
        std::cout << "Enter Article ID to dislike: ";
        std::cin >> id;
        std::cin.ignore();
        std::cout << "Disliked Article ID: " << id << " (stub, backend not implemented yet)\n";
    } else if (action == 6) {
        int id;
        std::cout << "Enter Article ID to report: ";
        std::cin >> id;
        std::cin.ignore();
        nlohmann::json body = { {"user_id", userSession.getUserId()}, {"article_id", id} };
        std::string res = httpClient.post("/articles/report", body.dump());
        std::cout << "Server: " << res << "\n";
    } else if (action == 2) {
        exit(0);
    }
}
