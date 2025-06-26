#ifndef SAVED_ARTICLE_CONTROLLER_H
#define SAVED_ARTICLE_CONTROLLER_H

#include <lib/crow/crow_all.h>
#include <memory>
#include "database/inc/DBConnection.h"
#include "models/inc/NewsArticle.h"

class SavedArticleController {
public:
    static crow::response save(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getSavedArticles(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response deleteSavedArticle(const crow::request& req, std::shared_ptr<DBConnection> dbConn, int articleId);

};

#endif