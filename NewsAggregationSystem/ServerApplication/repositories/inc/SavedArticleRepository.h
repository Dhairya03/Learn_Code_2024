#ifndef SAVED_ARTICLE_REPOSITORY_H
#define SAVED_ARTICLE_REPOSITORY_H

#include "models/inc/SavedArticle.h"
#include <memory>
#include <vector>
#include "models/inc/NewsArticle.h"
#include "database/inc/DBConnection.h"

class SavedArticleRepository {
public:
    explicit SavedArticleRepository(std::shared_ptr<DBConnection> dbConn);
    bool saveArticleForUser(int userId, int articleId);
    std::vector<NewsArticle> getSavedArticlesByUser(int userId);
    bool deleteSavedArticle(int userId, int articleId);

private:
    std::shared_ptr<DBConnection> db;
};

#endif