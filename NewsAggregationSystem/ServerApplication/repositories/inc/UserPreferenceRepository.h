#ifndef USER_PREFERENCE_REPOSITORY_H
#define USER_PREFERENCE_REPOSITORY_H

#include "models/inc/UserPreference.h"
#include "models/inc/NewsArticle.h"
#include <memory>
#include <vector>
#include "database/inc/DBConnection.h"

class UserPreferenceRepository {
public:
    explicit UserPreferenceRepository(std::shared_ptr<DBConnection> dbConn);
    
    UserPreference getUserPreference(int userId);
    bool updateUserPreference(const UserPreference& preference);
    
    bool trackArticleRead(int userId, int articleId);
    std::vector<int> getUserReadArticles(int userId);
    std::vector<int> getUserLikedArticles(int userId);
    std::vector<int> getUserSavedArticles(int userId);
    
    std::vector<NewsArticle> getPersonalizedArticles(int userId, int limit = 20);
    std::vector<NewsArticle> getRecommendedArticles(int userId, int limit = 10);
    
    double calculateArticleScore(const NewsArticle& article, const UserPreference& preference);
    std::vector<NewsArticle> rankArticlesByPreference(const std::vector<NewsArticle>& articles, const UserPreference& preference);

private:
    std::shared_ptr<DBConnection> db;
    void createTablesIfNotExist();
    std::vector<NewsArticle> getArticlesByCategory(const std::vector<std::string>& categories);
    std::vector<NewsArticle> getArticlesByKeywords(const std::vector<std::string>& keywords);
    std::vector<NewsArticle> getSimilarArticles(const std::vector<int>& articleIds);
};

#endif 