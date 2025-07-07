#include "../inc/SearchMenu.h"
#include "../../Services/inc/ArticleService.h"
#include <iostream>
#include <limits>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SearchMenu::SearchMenu(Client& c, Session& s) : client(c), session(s), articleService(c) {}

void SearchMenu::display() {
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
    std::cout << "\nResults for '" << query << "':\n";
    int idx = 1;
    for (const auto& article : results) {
        std::cout << idx << ". " << article["title"] << "\n   Source: " << article["source"] << " | Likes: " << article["likes"] << " | Dislikes: " << article["dislikes"] << "\n";
        ++idx;
    }
    std::cout << "\nEnter article number to save, or 0 to go back: ";
    int choice = 0;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (choice > 0 && choice <= (int)results.size()) {
        int articleId = results[choice-1]["id"];
        json body = { {"user_id", session.getUserId()}, {"article_id", articleId} };
        std::string res = client.post("/user/articles/save", body.dump());
        std::cout << "Server: " << res << "\n";
    }
}
