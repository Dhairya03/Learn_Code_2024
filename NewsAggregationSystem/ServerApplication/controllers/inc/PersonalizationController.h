#ifndef PERSONALIZATION_CONTROLLER_H
#define PERSONALIZATION_CONTROLLER_H

#include <lib/crow/crow_all.h>
#include <memory>
#include "database/inc/DBConnection.h"

class PersonalizationController {
public:
    static crow::response getPersonalizedArticles(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getRecommendedArticles(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getTrendingArticles(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response trackArticleView(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response trackArticleInteraction(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getUserInterests(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getRecommendedCategories(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
};

#endif 