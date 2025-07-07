#ifndef NEWS_CONTROLLER_H
#define NEWS_CONTROLLER_H

#include <lib/crow/crow_all.h>
#include <memory>
#include "database/inc/DBConnection.h"

class NewsController {
public:
    static crow::response getAll(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getByCategory(const crow::request& req, std::shared_ptr<DBConnection> dbConn, const std::string& categoryName);
    static crow::response search(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getTodayNews(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getNewsByDateAndCategory(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response searchNews(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
};

#endif