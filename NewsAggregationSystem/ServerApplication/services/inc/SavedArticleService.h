#ifndef SAVED_ARTICLE_SERVICE_H
#define SAVED_ARTICLE_SERVICE_H

#include <memory>
#include <vector>
#include "database/inc/DBConnection.h"
#include "models/inc/NewsArticle.h"

class SavedArticleService {
public:
    explicit SavedArticleService(std::shared_ptr<DBConnection> dbConn);
    bool saveArticle(int userId, int articleId);
    std::vector<NewsArticle> getSavedArticles(int userId);
    bool deleteSavedArticle(int userId, int articleId);

private:
    std::shared_ptr<DBConnection> db;
};

#endif