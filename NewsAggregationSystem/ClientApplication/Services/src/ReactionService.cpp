#include "../inc/ReactionService.h"
#include <iostream>

ReactionService::ReactionService(Client& c, Session& s) : client(c), session(s) {}

bool ReactionService::likeArticle(int articleId) {
    try {
        nlohmann::json body = {
            {"user_id", session.getUserId()},
            {"article_id", articleId}
        };
        
        std::string response = client.post("/articles/like", body.dump());
        auto responseJson = nlohmann::json::parse(response);
        
        if (responseJson.contains("success") && responseJson["success"].get<bool>()) {
            std::cout << "Article liked successfully!" << std::endl;
            return true;
        } else {
            std::cout << "Failed to like article: " << responseJson.value("message", "Unknown error") << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error liking article: " << e.what() << std::endl;
        return false;
    }
}

bool ReactionService::dislikeArticle(int articleId) {
    try {
        nlohmann::json body = {
            {"user_id", session.getUserId()},
            {"article_id", articleId}
        };
        
        std::string response = client.post("/articles/dislike", body.dump());
        auto responseJson = nlohmann::json::parse(response);
        
        if (responseJson.contains("success") && responseJson["success"].get<bool>()) {
            std::cout << "Article disliked successfully!" << std::endl;
            return true;
        } else {
            std::cout << "Failed to dislike article: " << responseJson.value("message", "Unknown error") << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error disliking article: " << e.what() << std::endl;
        return false;
    }
}

bool ReactionService::removeReaction(int articleId) {
    try {
        nlohmann::json body = {
            {"user_id", session.getUserId()},
            {"article_id", articleId}
        };
        
        std::string response = client.post("/articles/reaction/remove", body.dump());
        auto responseJson = nlohmann::json::parse(response);
        
        if (responseJson.contains("success") && responseJson["success"].get<bool>()) {
            std::cout << "Reaction removed successfully!" << std::endl;
            return true;
        } else {
            std::cout << "Failed to remove reaction: " << responseJson.value("message", "Unknown error") << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error removing reaction: " << e.what() << std::endl;
        return false;
    }
}

std::string ReactionService::getUserReaction(int articleId) {
    try {
        std::string endpoint = "/articles/reaction/user?user_id=" + std::to_string(session.getUserId()) + 
                              "&article_id=" + std::to_string(articleId);
        
        std::string response = client.get(endpoint);
        auto responseJson = nlohmann::json::parse(response);
        
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data") && responseJson["data"].contains("reaction")) {
            return responseJson["data"]["reaction"].get<std::string>();
        } else {
            return "NONE";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting user reaction: " << e.what() << std::endl;
        return "NONE";
    }
}

int ReactionService::getLikeCount(int articleId) {
    try {
        std::string endpoint = "/articles/reactions?article_id=" + std::to_string(articleId);
        std::string response = client.get(endpoint);
        auto responseJson = nlohmann::json::parse(response);
        
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data") && responseJson["data"].contains("likes")) {
            return responseJson["data"]["likes"].get<int>();
        } else {
            return 0;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting like count: " << e.what() << std::endl;
        return 0;
    }
}

int ReactionService::getDislikeCount(int articleId) {
    try {
        std::string endpoint = "/articles/reactions?article_id=" + std::to_string(articleId);
        std::string response = client.get(endpoint);
        auto responseJson = nlohmann::json::parse(response);
        
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data") && responseJson["data"].contains("dislikes")) {
            return responseJson["data"]["dislikes"].get<int>();
        } else {
            return 0;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting dislike count: " << e.what() << std::endl;
        return 0;
    }
} 