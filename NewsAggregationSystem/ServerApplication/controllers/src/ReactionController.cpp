#include "controllers/inc/ReactionController.h"
#include "services/inc/ReactionService.h"
#include "lib/json/json.hpp"
#include <iostream>

using json = nlohmann::json;

crow::response ReactionController::likeArticle(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[ReactionController] likeArticle called" << std::endl;
    
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
        
        ReactionService reactionService(dbConn);
        bool success = reactionService.likeArticle(userId, articleId);
        
        if (success) {
            json response = {
                {"success", true},
                {"message", "Article liked successfully"}
            };
            return crow::response(200, response.dump());
        } else {
            json response = {
                {"success", false},
                {"message", "Failed to like article"}
            };
            return crow::response(500, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[ReactionController] likeArticle error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
}

crow::response ReactionController::dislikeArticle(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[ReactionController] dislikeArticle called" << std::endl;
    
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
        
        ReactionService reactionService(dbConn);
        bool success = reactionService.dislikeArticle(userId, articleId);
        
        if (success) {
            json response = {
                {"success", true},
                {"message", "Article disliked successfully"}
            };
            return crow::response(200, response.dump());
        } else {
            json response = {
                {"success", false},
                {"message", "Failed to dislike article"}
            };
            return crow::response(500, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[ReactionController] dislikeArticle error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
}

crow::response ReactionController::removeReaction(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[ReactionController] removeReaction called" << std::endl;
    
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
        
        ReactionService reactionService(dbConn);
        bool success = reactionService.removeReaction(userId, articleId);
        
        if (success) {
            json response = {
                {"success", true},
                {"message", "Reaction removed successfully"}
            };
            return crow::response(200, response.dump());
        } else {
            json response = {
                {"success", false},
                {"message", "Failed to remove reaction"}
            };
            return crow::response(500, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[ReactionController] removeReaction error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
}

crow::response ReactionController::getUserReaction(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[ReactionController] getUserReaction called" << std::endl;
    
    try {
        int userId = std::stoi(req.url_params.get("user_id"));
        int articleId = std::stoi(req.url_params.get("article_id"));
        
        ReactionService reactionService(dbConn);
        ReactionType reaction = reactionService.getUserReaction(userId, articleId);
        
        std::string reactionString;
        switch (reaction) {
            case ReactionType::LIKE:
                reactionString = "LIKE";
                break;
            case ReactionType::DISLIKE:
                reactionString = "DISLIKE";
                break;
            default:
                reactionString = "NONE";
                break;
        }
        
        json response = {
            {"success", true},
            {"data", {
                {"user_id", userId},
                {"article_id", articleId},
                {"reaction", reactionString}
            }}
        };
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[ReactionController] getUserReaction error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
}

crow::response ReactionController::getArticleReactions(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[ReactionController] getArticleReactions called" << std::endl;
    
    try {
        int articleId = std::stoi(req.url_params.get("article_id"));
        
        ReactionService reactionService(dbConn);
        int likeCount = reactionService.getLikeCount(articleId);
        int dislikeCount = reactionService.getDislikeCount(articleId);
        
        json response = {
            {"success", true},
            {"data", {
                {"article_id", articleId},
                {"likes", likeCount},
                {"dislikes", dislikeCount}
            }}
        };
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[ReactionController] getArticleReactions error: " << e.what() << std::endl;
        json response = {
            {"success", false},
            {"message", "Internal server error"}
        };
        return crow::response(500, response.dump());
    }
} 