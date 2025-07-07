#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include <lib/crow/crow_all.h>
#include <memory>
#include "database/inc/DBConnection.h"

class AdminController {
public:
    static crow::response getExternalServers(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getExternalServerDetails(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response updateApiKey(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response addCategory(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getReportedArticles(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response hideArticle(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response unhideArticle(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response hideCategory(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response unhideCategory(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
};

#endif