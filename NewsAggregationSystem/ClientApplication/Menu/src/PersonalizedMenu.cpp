#include "../inc/PersonalizedMenu.h"
#include "../../Services/inc/SavedArticleService.h"
#include "../../Services/inc/ReactionService.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

PersonalizedMenu::PersonalizedMenu(Client& httpClient, Session& userSession)
    : httpClient(httpClient), userSession(userSession), personalizationService(httpClient, userSession) {}

void PersonalizedMenu::display() {
    while (true) {
        std::cout << "\nP E R S O N A L I Z E D - C O N T E N T\n"
                  << "1. Personalized Articles\n"
                  << "2. Recommended Articles\n"
                  << "3. Trending Articles\n"
                  << "4. My Interests & Insights\n"
                  << "5. Back\n"
                  << "6. Logout\n"
                  << "Enter your choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                displayPersonalizedArticles();
                break;
            case 2:
                displayRecommendedArticles();
                break;
            case 3:
                displayTrendingArticles();
                break;
            case 4:
                displayUserInsights();
                break;
            case 5:
                return;
            case 6:
                exit(0);
            default:
                std::cout << "Invalid option.\n";
        }
    }
}

void PersonalizedMenu::displayPersonalizedArticles() {
    std::cout << "\nLoading personalized articles based on your preferences...\n";
    auto articles = personalizationService.getPersonalizedArticles(15);
    displayArticleTable(articles, "Personalized Articles");
}

void PersonalizedMenu::displayRecommendedArticles() {
    std::cout << "\nLoading recommended articles for you...\n";
    auto articles = personalizationService.getRecommendedArticles(10);
    displayArticleTable(articles, "Recommended Articles");
}

void PersonalizedMenu::displayTrendingArticles() {
    std::cout << "\nLoading trending articles personalized for you...\n";
    auto articles = personalizationService.getTrendingArticles(10);
    displayArticleTable(articles, "Trending Articles");
}

void PersonalizedMenu::displayUserInsights() {
    std::cout << "\nY O U R - I N S I G H T S\n";
    
    auto interests = personalizationService.getUserInterests();
    std::cout << "Your Interests:\n";
    if (interests.empty()) {
        std::cout << "  No interests detected yet. Start reading articles to build your profile!\n";
    } else {
        for (const auto& interest : interests) {
            std::cout << "  • " << interest << "\n";
        }
    }
    
    auto recommendedCategories = personalizationService.getRecommendedCategories();
    std::cout << "\nRecommended Categories:\n";
    if (recommendedCategories.empty()) {
        std::cout << "  You have all categories enabled!\n";
    } else {
        for (const auto& category : recommendedCategories) {
            std::cout << "  • " << category << "\n";
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void PersonalizedMenu::displayArticleTable(const std::vector<json>& articles, const std::string& title) {
    if (articles.empty()) {
        std::cout << "\nNo " << title << " found. Try adjusting your preferences.\n";
        return;
    }

    std::cout << "\n" << title << ":\n";
    std::cout << "+-----+----------------------------------------------------------+-------------------+--------+----------+\n";
    std::cout << "| ID  | Title                                                    | Source            | Likes  | Dislikes |\n";
    std::cout << "+-----+----------------------------------------------------------+-------------------+--------+----------+\n";
    
    for (const auto& article : articles) {
        std::cout << "| " << std::setw(3) << article["id"] << " | "
                  << std::setw(58) << article["title"].get<std::string>().substr(0,58) << " | "
                  << std::setw(17) << article["source"].get<std::string>().substr(0,17) << " | "
                  << std::setw(6) << article["likes"] << " | "
                  << std::setw(8) << article["dislikes"] << " |\n";
    }
    std::cout << "+-----+----------------------------------------------------------+-------------------+--------+----------+\n";

    std::cout << "\nOptions:\n";
    std::cout << "1. View article details\n";
    std::cout << "2. Save article\n";
    std::cout << "3. Like article\n";
    std::cout << "4. Dislike article\n";
    std::cout << "5. Back\n";
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 1) {
        int articleId;
        std::cout << "Enter Article ID to view: ";
        std::cin >> articleId;
        std::cin.ignore();

        for (const auto& article : articles) {
            if (article["id"] == articleId) {
                std::cout << "\nArticle Details:\n";
                std::cout << "Title: " << article["title"] << "\n";
                std::cout << "Description: " << article["description"] << "\n";
                std::cout << "Source: " << article["source"] << "\n";
                std::cout << "URL: " << article["url"] << "\n";
                std::cout << "Category: " << article["category"] << "\n";
                std::cout << "Likes: " << article["likes"] << " | Dislikes: " << article["dislikes"] << "\n";

                personalizationService.trackArticleView(articleId);
                personalizationService.trackArticleInteraction(articleId, "view");
                
                handleArticleInteraction(article);
                break;
            }
        }
    } else if (choice >= 2 && choice <= 4) {
        int articleId;
        std::cout << "Enter Article ID: ";
        std::cin >> articleId;
        std::cin.ignore();

        for (const auto& article : articles) {
            if (article["id"] == articleId) {
                if (choice == 2) {
                    SavedArticleService(httpClient, userSession).saveArticle(articleId);
                    personalizationService.trackArticleInteraction(articleId, "save");
                } else if (choice == 3) {
                    ReactionService(httpClient, userSession).likeArticle(articleId);
                    personalizationService.trackArticleInteraction(articleId, "like");
                } else if (choice == 4) {
                    ReactionService(httpClient, userSession).dislikeArticle(articleId);
                    personalizationService.trackArticleInteraction(articleId, "dislike");
                }
                break;
            }
        }
    }
}

void PersonalizedMenu::handleArticleInteraction(const json& article) {
    std::cout << "\nWhat would you like to do with this article?\n";
    std::cout << "1. Save article\n";
    std::cout << "2. Like article\n";
    std::cout << "3. Dislike article\n";
    std::cout << "4. Back to list\n";
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    int articleId = article["id"];
    
    switch (choice) {
        case 1:
            SavedArticleService(httpClient, userSession).saveArticle(articleId);
            personalizationService.trackArticleInteraction(articleId, "save");
            std::cout << "Article saved!\n";
            break;
        case 2:
            ReactionService(httpClient, userSession).likeArticle(articleId);
            personalizationService.trackArticleInteraction(articleId, "like");
            break;
        case 3:
            ReactionService(httpClient, userSession).dislikeArticle(articleId);
            personalizationService.trackArticleInteraction(articleId, "dislike");
            break;
        case 4:
            break;
        default:
            std::cout << "Invalid choice.\n";
    }
} 