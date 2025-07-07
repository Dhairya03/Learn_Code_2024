#ifndef NEWS_SERVICE_H
#define NEWS_SERVICE_H

#include "models/inc/NewsArticle.h"
#include <vector>
#include <memory>
#include "database/inc/DBConnection.h"

class NewsService {
public:
    explicit NewsService(std::shared_ptr<DBConnection> dbConn);
    std::vector<NewsArticle> getAllArticles();
    std::vector<NewsArticle> getArticlesByCategory(const std::string& category);
    std::vector<NewsArticle> searchArticles(const std::string& query, const std::string& startDate, const std::string& endDate, const std::string& sort);
    std::vector<NewsArticle> getNewsByDateAndCategory(const std::string& startDate, const std::string& endDate, const std::string& category);
    std::vector<NewsArticle> getArticlesToday();

private:
    std::shared_ptr<DBConnection> db;
};

#endif