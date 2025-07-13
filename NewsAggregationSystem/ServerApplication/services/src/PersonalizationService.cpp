#include "services/inc/PersonalizationService.h"
#include "repositories/inc/UserPreferenceRepository.h"
#include <iostream>
#include <algorithm>

PersonalizationService::PersonalizationService(std::shared_ptr<DBConnection> dbConn) : db(std::move(dbConn)) {}

std::vector<NewsArticle> PersonalizationService::getPersonalizedArticles(int userId, int limit) {
    std::cout << "[PersonalizationService] getPersonalizedArticles called for user " << userId << std::endl;
    try {
        UserPreferenceRepository repo(db);
        auto articles = repo.getPersonalizedArticles(userId, limit);
        std::cout << "[PersonalizationService] getPersonalizedArticles success, returned " << articles.size() << " articles" << std::endl;
        return articles;
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationService] getPersonalizedArticles error: " << e.what() << std::endl;
        return {};
    }
}

std::vector<NewsArticle> PersonalizationService::getRecommendedArticles(int userId, int limit) {
    std::cout << "[PersonalizationService] getRecommendedArticles called for user " << userId << std::endl;
    try {
        UserPreferenceRepository repo(db);
        auto articles = repo.getRecommendedArticles(userId, limit);
        std::cout << "[PersonalizationService] getRecommendedArticles success, returned " << articles.size() << " articles" << std::endl;
        return articles;
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationService] getRecommendedArticles error: " << e.what() << std::endl;
        return {};
    }
}

std::vector<NewsArticle> PersonalizationService::getTrendingArticles(int userId, int limit) {
    std::cout << "[PersonalizationService] getTrendingArticles called for user " << userId << std::endl;
    try {
        UserPreferenceRepository repo(db);
        auto preference = repo.getUserPreference(userId);
        auto articles = repo.getPersonalizedArticles(userId, limit * 2);
        
        std::sort(articles.begin(), articles.end(), 
            [](const NewsArticle& a, const NewsArticle& b) {
                int aScore = a.likes - a.dislikes;
                int bScore = b.likes - b.dislikes;
                return aScore > bScore;
            });
        
        if (articles.size() > static_cast<size_t>(limit)) {
            articles.resize(limit);
        }
        
        std::cout << "[PersonalizationService] getTrendingArticles success, returned " << articles.size() << " articles" << std::endl;
        return articles;
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationService] getTrendingArticles error: " << e.what() << std::endl;
        return {};
    }
}

bool PersonalizationService::trackArticleView(int userId, int articleId) {
    std::cout << "[PersonalizationService] trackArticleView called for user " << userId << ", article " << articleId << std::endl;
    try {
        UserPreferenceRepository repo(db);
        bool result = repo.trackArticleRead(userId, articleId);
        std::cout << "[PersonalizationService] trackArticleView success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationService] trackArticleView error: " << e.what() << std::endl;
        return false;
    }
}

bool PersonalizationService::trackArticleInteraction(int userId, int articleId, const std::string& interactionType) {
    std::cout << "[PersonalizationService] trackArticleInteraction called for user " << userId 
              << ", article " << articleId << ", type: " << interactionType << std::endl;
    try {
        UserPreferenceRepository repo(db);
        
        if (interactionType == "like" || interactionType == "dislike" || interactionType == "save") {
            repo.trackArticleRead(userId, articleId);
        } else {
            repo.trackArticleRead(userId, articleId);
        }
        
        std::cout << "[PersonalizationService] trackArticleInteraction success" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationService] trackArticleInteraction error: " << e.what() << std::endl;
        return false;
    }
}

UserPreference PersonalizationService::getUserPreference(int userId) {
    try {
        UserPreferenceRepository repo(db);
        return repo.getUserPreference(userId);
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationService] getUserPreference error: " << e.what() << std::endl;
        UserPreference preference;
        preference.userId = userId;
        return preference;
    }
}

bool PersonalizationService::updateUserPreference(const UserPreference& preference) {
    std::cout << "[PersonalizationService] updateUserPreference called for user " << preference.userId << std::endl;
    try {
        UserPreferenceRepository repo(db);
        bool result = repo.updateUserPreference(preference);
        std::cout << "[PersonalizationService] updateUserPreference success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationService] updateUserPreference error: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> PersonalizationService::getUserInterests(int userId) {
    std::vector<std::string> interests;
    try {
        UserPreferenceRepository repo(db);
        auto preference = repo.getUserPreference(userId);
        
        auto categories = preference.getEnabledCategories();
        interests.insert(interests.end(), categories.begin(), categories.end());
        
        auto keywords = preference.getKeywordsList();
        interests.insert(interests.end(), keywords.begin(), keywords.end());
        
        if (!preference.likedArticleIds.empty()) {
            interests.push_back("Likes articles");
        }
        if (!preference.savedArticleIds.empty()) {
            interests.push_back("Saves articles");
        }
        if (!preference.readArticleIds.empty()) {
            interests.push_back("Active reader");
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationService] getUserInterests error: " << e.what() << std::endl;
    }
    
    return interests;
}

std::vector<std::string> PersonalizationService::getRecommendedCategories(int userId) {
    std::vector<std::string> recommendedCategories;
    try {
        UserPreferenceRepository repo(db);
        auto preference = repo.getUserPreference(userId);
        
        auto enabledCategories = preference.getEnabledCategories();
        
        if (!preference.likedArticleIds.empty() || !preference.savedArticleIds.empty()) {
            std::vector<std::string> allCategories = {"business", "entertainment", "sports", "technology"};
            
            for (const auto& category : allCategories) {
                if (std::find(enabledCategories.begin(), enabledCategories.end(), category) == enabledCategories.end()) {
                    recommendedCategories.push_back(category);
                }
            }
        } else {
            recommendedCategories = {"technology", "business", "entertainment"};
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationService] getRecommendedCategories error: " << e.what() << std::endl;
    }
    
    return recommendedCategories;
} 