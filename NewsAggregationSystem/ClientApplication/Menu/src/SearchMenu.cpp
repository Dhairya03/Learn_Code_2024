#include "../inc/SearchMenu.h"
#include "../../Services/inc/ArticleService.h"
#include <iostream>
#include <limits>
#include <nlohmann/json.hpp>
#include <iomanip>

using json = nlohmann::json;

SearchMenu::SearchMenu(Client& httpClient, Session& userSession)
    : httpClient(httpClient), userSession(userSession), articleService(httpClient) {}

void SearchMenu::display() {
    displaySearchResults();
}

void SearchMenu::displaySearchResults() {
    std::string query, startDate, endDate, sort;
    std::cout << "\nS E A R C H\n";
    std::cout << "Enter search query: ";
    std::getline(std::cin, query);
    std::cout << "Enter start date (YYYY-MM-DD, optional): ";
    std::getline(std::cin, startDate);
    std::cout << "Enter end date (YYYY-MM-DD, optional): ";
    std::getline(std::cin, endDate);
    std::cout << "Sort by (likes/dislikes/newest): ";
    std::getline(std::cin, sort);
    if (sort != "likes" && sort != "dislikes") sort = "";

    auto results = articleService.searchArticles(query, startDate, endDate, sort);
    if (results.empty()) {
        std::cout << "No articles found.\n";
        return;
    }
    std::cout << "\n+-----+----------------------------------------------------------+-------------------+--------+----------+\n";
    std::cout << "| ID  | Title                                                    | Source            | Likes  | Dislikes |\n";
    std::cout << "+-----+----------------------------------------------------------+-------------------+--------+----------+\n";
    for (const auto& article : results) {
        std::cout << "| " << std::setw(3) << article["id"] << " | "
                  << std::setw(58) << article["title"].get<std::string>().substr(0,58) << " | "
                  << std::setw(17) << article["source"].get<std::string>().substr(0,17) << " | "
                  << std::setw(6) << article["likes"] << " | "
                  << std::setw(8) << article["dislikes"] << " |\n";
    }
    std::cout << "+-----+----------------------------------------------------------+-------------------+--------+----------+\n";
    std::cout << "\nEnter article number to save, or 0 to go back: ";
    int choice = 0;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (choice > 0 && choice <= (int)results.size()) {
        int articleId = results[choice-1]["id"];
        json body = { {"user_id", userSession.getUserId()}, {"article_id", articleId} };
        std::string res = httpClient.post("/user/articles/save", body.dump());
        std::cout << "Server: " << res << "\n";
    }
}
