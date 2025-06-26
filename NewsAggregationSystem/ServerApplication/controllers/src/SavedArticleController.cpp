#include "controllers/inc/SavedArticleController.h"
#include "services/inc/SavedArticleService.h"

#include "lib/json/json.hpp"

using json = nlohmann::json;

crow::response SavedArticleController::save(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    try {
        auto body = json::parse(req.body);
        if (!body.contains("user_id") || !body.contains("article_id")) {
            return crow::response(400, "Missing user_id or article_id");
        }

        int userId = body["user_id"];
        int articleId = body["article_id"];

        SavedArticleService service(dbConn);
        bool success = service.saveArticle(userId, articleId);

        if (success)
            return crow::response(200, "Article saved");
        else
            return crow::response(500, "Failed to save article");

    } catch (...) {
        return crow::response(400, "Invalid JSON");
    }
}

crow::response SavedArticleController::getSavedArticles(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    auto urlParams = crow::query_string(req.url_params);
    if (!urlParams.get("user_id")) {
        return crow::response(400, "Missing user_id");
    }

    int userId = std::stoi(urlParams.get("user_id"));
    SavedArticleService service(dbConn);
    std::vector<NewsArticle> articles = service.getSavedArticles(userId);

    nlohmann::json result = nlohmann::json::array();
    for (const auto& a : articles) {
        nlohmann::json articleJson = {
            {"id", a.id},
            {"title", a.title},
            {"description", a.description},
            {"url", a.url},
            {"source", a.source},
            {"published_at", a.publishedAt}
        };
        result.push_back(articleJson);
    }

    return crow::response(200, result.dump());
}

crow::response SavedArticleController::deleteSavedArticle(const crow::request& req, std::shared_ptr<DBConnection> dbConn, int articleId) {
    auto urlParams = crow::query_string(req.url_params);
    if (!urlParams.get("user_id")) {
        return crow::response(400, "Missing user_id");
    }

    int userId = std::stoi(urlParams.get("user_id"));
    SavedArticleService service(dbConn);
    bool success = service.deleteSavedArticle(userId, articleId);

    if (success)
        return crow::response(200, "Article unsaved");
    else
        return crow::response(500, "Failed to unsave article");
}
