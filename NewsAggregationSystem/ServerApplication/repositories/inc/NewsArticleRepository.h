#ifndef NEWS_ARTICLE_REPOSITORY_H
#define NEWS_ARTICLE_REPOSITORY_H

#include "models/inc/NewsArticle.h"
#include <vector>
#include <memory>
#include "database/inc/DBConnection.h"

class NewsArticleRepository {
public:
    explicit NewsArticleRepository(std::shared_ptr<DBConnection> dbConn);
    std::vector<NewsArticle> getAllArticles();
    std::vector<NewsArticle> getArticlesByCategory(const std::string& categoryName);
    bool saveArticle(const NewsArticle& article);
    std::vector<NewsArticle> searchArticles(const std::string& keyword,
        const std::string& startDate = "",
        const std::string& endDate = "",
        const std::string& sortField = "");
    std::vector<NewsArticle> getArticlesToday();
    std::vector<NewsArticle> getArticlesByDateAndCategory(const std::string& startDate, const std::string& endDate, const std::string& categoryName);

private:
    std::shared_ptr<DBConnection> db;
};

#endif
