#include "controllers/inc/SavedArticleController.h"
#include "services/inc/SavedArticleService.h"

#include "lib/json/json.hpp"
#include <iostream>

using json = nlohmann::json;

crow::response SavedArticleController::save(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[SavedArticleController] save called" << std::endl;
    try {
        auto body = json::parse(req.body);
        if (!body.contains("user_id") || !body.contains("article_id")) {
            json response = {{"success", false}, {"message", "Missing user_id or article_id"}};
            return crow::response(400, response.dump());
        }
        int userId = body["user_id"];
        int articleId = body["article_id"];
        SavedArticleService service(dbConn);
        bool success = service.saveArticle(userId, articleId);
        if (success) {
            json response = {{"success", true}, {"message", "Article saved"}};
            std::cout << "[SavedArticleController] save success" << std::endl;
            return crow::response(200, response.dump());
        } else {
            json response = {{"success", false}, {"message", "Failed to save article"}};
            std::cerr << "[SavedArticleController] save failed" << std::endl;
            return crow::response(500, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[SavedArticleController] save error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(400, response.dump());
    } catch (...) {
        std::cerr << "[SavedArticleController] save unknown error" << std::endl;
        json response = {{"success", false}, {"message", "Invalid JSON"}};
        return crow::response(400, response.dump());
    }
}

crow::response SavedArticleController::getSavedArticles(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[SavedArticleController] getSavedArticles called" << std::endl;
    try {
        auto urlParams = crow::query_string(req.url_params);
        if (!urlParams.get("user_id")) {
            json response = {{"success", false}, {"message", "Missing user_id"}};
            return crow::response(400, response.dump());
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
        json response = {{"success", true}, {"data", result}};
        std::cout << "[SavedArticleController] getSavedArticles success" << std::endl;
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[SavedArticleController] getSavedArticles error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}

crow::response SavedArticleController::deleteSavedArticle(const crow::request& req, std::shared_ptr<DBConnection> dbConn, int articleId) {
    std::cout << "[SavedArticleController] deleteSavedArticle called" << std::endl;
    try {
        auto urlParams = crow::query_string(req.url_params);
        if (!urlParams.get("user_id")) {
            json response = {{"success", false}, {"message", "Missing user_id"}};
            return crow::response(400, response.dump());
        }
        int userId = std::stoi(urlParams.get("user_id"));
        SavedArticleService service(dbConn);
        bool success = service.deleteSavedArticle(userId, articleId);
        if (success) {
            json response = {{"success", true}, {"message", "Article unsaved"}};
            std::cout << "[SavedArticleController] deleteSavedArticle success" << std::endl;
            return crow::response(200, response.dump());
        } else {
            json response = {{"success", false}, {"message", "Failed to unsave article"}};
            std::cerr << "[SavedArticleController] deleteSavedArticle failed" << std::endl;
            return crow::response(500, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[SavedArticleController] deleteSavedArticle error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}
