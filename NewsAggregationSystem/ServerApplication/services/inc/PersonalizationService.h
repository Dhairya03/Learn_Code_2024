#ifndef PERSONALIZATION_SERVICE_H
#define PERSONALIZATION_SERVICE_H

#include "models/inc/NewsArticle.h"
#include "models/inc/UserPreference.h"
#include <memory>
#include <vector>
#include "database/inc/DBConnection.h"

class PersonalizationService {
public:
    explicit PersonalizationService(std::shared_ptr<DBConnection> dbConn);
    
    std::vector<NewsArticle> getPersonalizedArticles(int userId, int limit = 20);
    std::vector<NewsArticle> getRecommendedArticles(int userId, int limit = 10);
    std::vector<NewsArticle> getTrendingArticles(int userId, int limit = 10);
    
    bool trackArticleView(int userId, int articleId);
    bool trackArticleInteraction(int userId, int articleId, const std::string& interactionType);
    
    UserPreference getUserPreference(int userId);
    bool updateUserPreference(const UserPreference& preference);
    
    std::vector<std::string> getUserInterests(int userId);
    std::vector<std::string> getRecommendedCategories(int userId);

private:
    std::shared_ptr<DBConnection> db;
};

#endif 