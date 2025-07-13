#include "controllers/inc/PersonalizationController.h"
#include "services/inc/PersonalizationService.h"
#include "lib/json/json.hpp"
#include <iostream>

using json = nlohmann::json;

crow::response PersonalizationController::getPersonalizedArticles(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[PersonalizationController] getPersonalizedArticles called" << std::endl;
    
    try {
        int userId = std::stoi(req.url_params.get("user_id"));
        int limit = 20;
        if (req.url_params.get("limit")) {
            limit = std::stoi(req.url_params.get("limit"));
        }
        
        PersonalizationService service(dbConn);
        auto articles = service.getPersonalizedArticles(userId, limit);
        
        json articlesArray = json::array();
        for (const auto& article : articles) {
            json articleJson = {
                {"id", article.id},
                {"title", article.title},
                {"description", article.description},
                {"url", article.url},
                {"source", article.source},
                {"published_at", article.publishedAt},
                {"category", article.categoryName},
                {"categoryId", article.categoryId},
                {"likes", article.likes},
                {"dislikes", article.dislikes}
            };
            articlesArray.push_back(articleJson);
        }
        
        json response = {
            {"success", true},
            {"data", articlesArray}
        };
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationController] getPersonalizedArticles error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
}

crow::response PersonalizationController::getRecommendedArticles(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[PersonalizationController] getRecommendedArticles called" << std::endl;
    
    try {
        int userId = std::stoi(req.url_params.get("user_id"));
        int limit = 10;
        if (req.url_params.get("limit")) {
            limit = std::stoi(req.url_params.get("limit"));
        }
        
        PersonalizationService service(dbConn);
        auto articles = service.getRecommendedArticles(userId, limit);
        
        json articlesArray = json::array();
        for (const auto& article : articles) {
            json articleJson = {
                {"id", article.id},
                {"title", article.title},
                {"description", article.description},
                {"url", article.url},
                {"source", article.source},
                {"published_at", article.publishedAt},
                {"category", article.categoryName},
                {"categoryId", article.categoryId},
                {"likes", article.likes},
                {"dislikes", article.dislikes}
            };
            articlesArray.push_back(articleJson);
        }
        
        json response = {
            {"success", true},
            {"data", articlesArray}
        };
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationController] getRecommendedArticles error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
}

crow::response PersonalizationController::getTrendingArticles(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[PersonalizationController] getTrendingArticles called" << std::endl;
    
    try {
        int userId = std::stoi(req.url_params.get("user_id"));
        int limit = 10;
        if (req.url_params.get("limit")) {
            limit = std::stoi(req.url_params.get("limit"));
        }
        
        PersonalizationService service(dbConn);
        auto articles = service.getTrendingArticles(userId, limit);
        
        json articlesArray = json::array();
        for (const auto& article : articles) {
            json articleJson = {
                {"id", article.id},
                {"title", article.title},
                {"description", article.description},
                {"url", article.url},
                {"source", article.source},
                {"published_at", article.publishedAt},
                {"category", article.categoryName},
                {"categoryId", article.categoryId},
                {"likes", article.likes},
                {"dislikes", article.dislikes}
            };
            articlesArray.push_back(articleJson);
        }
        
        json response = {
            {"success", true},
            {"data", articlesArray}
        };
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationController] getTrendingArticles error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
}

crow::response PersonalizationController::trackArticleView(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[PersonalizationController] trackArticleView called" << std::endl;
    
    try {
        auto requestJson = json::parse(req.body);
        
        if (!requestJson.contains("user_id") || !requestJson.contains("article_id")) {
            json response = {
                {"success", false},
                {"message", "Missing required fields: user_id and article_id"}
            };
            return crow::response(400, response.dump());
        }
        
        int userId = requestJson["user_id"].get<int>();
        int articleId = requestJson["article_id"].get<int>();
        
        PersonalizationService service(dbConn);
        bool success = service.trackArticleView(userId, articleId);
        
        if (success) {
            json response = {
                {"success", true},
                {"message", "Article view tracked successfully"}
            };
            return crow::response(200, response.dump());
        } else {
            json response = {
                {"success", false},
                {"message", "Failed to track article view"}
            };
            return crow::response(500, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationController] trackArticleView error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
}

crow::response PersonalizationController::trackArticleInteraction(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[PersonalizationController] trackArticleInteraction called" << std::endl;
    
    try {
        auto requestJson = json::parse(req.body);
        
        if (!requestJson.contains("user_id") || !requestJson.contains("article_id") || !requestJson.contains("interaction_type")) {
            json response = {
                {"success", false},
                {"message", "Missing required fields: user_id, article_id, and interaction_type"}
            };
            return crow::response(400, response.dump());
        }
        
        int userId = requestJson["user_id"].get<int>();
        int articleId = requestJson["article_id"].get<int>();
        std::string interactionType = requestJson["interaction_type"].get<std::string>();
        
        PersonalizationService service(dbConn);
        bool success = service.trackArticleInteraction(userId, articleId, interactionType);
        
        if (success) {
            json response = {
                {"success", true},
                {"message", "Article interaction tracked successfully"}
            };
            return crow::response(200, response.dump());
        } else {
            json response = {
                {"success", false},
                {"message", "Failed to track article interaction"}
            };
            return crow::response(500, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationController] trackArticleInteraction error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
}

crow::response PersonalizationController::getUserInterests(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[PersonalizationController] getUserInterests called" << std::endl;
    
    try {
        int userId = std::stoi(req.url_params.get("user_id"));
        
        PersonalizationService service(dbConn);
        auto interests = service.getUserInterests(userId);
        
        json response = {
            {"success", true},
            {"data", {
                {"user_id", userId},
                {"interests", interests}
            }}
        };
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationController] getUserInterests error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
}

crow::response PersonalizationController::getRecommendedCategories(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[PersonalizationController] getRecommendedCategories called" << std::endl;
    
    try {
        int userId = std::stoi(req.url_params.get("user_id"));
        
        PersonalizationService service(dbConn);
        auto categories = service.getRecommendedCategories(userId);
        
        json response = {
            {"success", true},
            {"data", {
                {"user_id", userId},
                {"recommended_categories", categories}
            }}
        };
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[PersonalizationController] getRecommendedCategories error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
} 