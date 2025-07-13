#include "../inc/HeadlinesMenu.h"
#include "../../Services/inc/ArticleService.h"
#include "../../Services/inc/SavedArticleService.h"
#include "../../Services/inc/ReactionService.h"
#include "../../Services/inc/PersonalizationService.h"
#include "../../Services/inc/CategoryService.h"
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
    CategoryService categoryService(httpClient);
    auto categories = categoryService.getAllCategories();
    
    std::vector<std::string> allOptions = {"all"};
    allOptions.insert(allOptions.end(), categories.begin(), categories.end());
    
    std::cout << "\nPlease choose the options below for Headlines:\n";
    for (size_t i = 0; i < allOptions.size(); ++i) {
        std::cout << (i + 1) << ". " << allOptions[i] << "\n";
    }
    std::cout << ">> ";
    
    int opt;
    std::cin >> opt;
    std::cin.ignore();
    
    if (opt >= 1 && opt <= static_cast<int>(allOptions.size())) {
        return allOptions[opt - 1];
    } else {
        std::cout << "Invalid option, defaulting to 'all'\n";
        return "all";
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
    std::cout << "1. Back\n2. Logout\n3. Save Article\n4. Like Article\n5. Dislike Article\n6. Remove Reaction\n7. Report Article\n>> ";
    int action;
    std::cin >> action;
    std::cin.ignore();

    if (action == 3) {
        int id;
        std::cout << "Enter Article ID to save: ";
        std::cin >> id;
        std::cin.ignore();
        SavedArticleService(httpClient, userSession).saveArticle(id);
        PersonalizationService(httpClient, userSession).trackArticleInteraction(id, "save");
    } else if (action == 4) {
        int id;
        std::cout << "Enter Article ID to like: ";
        std::cin >> id;
        std::cin.ignore();
        ReactionService(httpClient, userSession).likeArticle(id);
        PersonalizationService(httpClient, userSession).trackArticleInteraction(id, "like");
    } else if (action == 5) {
        int id;
        std::cout << "Enter Article ID to dislike: ";
        std::cin >> id;
        std::cin.ignore();
        ReactionService(httpClient, userSession).dislikeArticle(id);
        PersonalizationService(httpClient, userSession).trackArticleInteraction(id, "dislike");
    } else if (action == 6) {
        int id;
        std::cout << "Enter Article ID to remove reaction: ";
        std::cin >> id;
        std::cin.ignore();
        ReactionService(httpClient, userSession).removeReaction(id);
    } else if (action == 7) {
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
