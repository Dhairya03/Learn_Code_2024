#ifndef REACTION_CONTROLLER_H
#define REACTION_CONTROLLER_H

#include <lib/crow/crow_all.h>
#include <memory>
#include "database/inc/DBConnection.h"

class ReactionController {
public:
    static crow::response likeArticle(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response dislikeArticle(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response removeReaction(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getUserReaction(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getArticleReactions(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
};

#endif 