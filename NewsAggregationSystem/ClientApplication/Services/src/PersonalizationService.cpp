#include "../inc/PersonalizationService.h"
#include <iostream>

PersonalizationService::PersonalizationService(Client& c, Session& s) : client(c), session(s) {}

std::vector<json> PersonalizationService::getPersonalizedArticles(int limit) {
    std::vector<json> articles;
    try {
        std::string endpoint = "/articles/personalized?user_id=" + std::to_string(session.getUserId()) + 
                              "&limit=" + std::to_string(limit);
        std::string response = client.get(endpoint);
        
        auto responseJson = json::parse(response);
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data")) {
            articles = responseJson["data"].get<std::vector<json>>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting personalized articles: " << e.what() << std::endl;
    }
    return articles;
}

std::vector<json> PersonalizationService::getRecommendedArticles(int limit) {
    std::vector<json> articles;
    try {
        std::string endpoint = "/articles/recommended?user_id=" + std::to_string(session.getUserId()) + 
                              "&limit=" + std::to_string(limit);
        std::string response = client.get(endpoint);
        
        auto responseJson = json::parse(response);
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data")) {
            articles = responseJson["data"].get<std::vector<json>>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting recommended articles: " << e.what() << std::endl;
    }
    return articles;
}

std::vector<json> PersonalizationService::getTrendingArticles(int limit) {
    std::vector<json> articles;
    try {
        std::string endpoint = "/articles/trending?user_id=" + std::to_string(session.getUserId()) + 
                              "&limit=" + std::to_string(limit);
        std::string response = client.get(endpoint);
        
        auto responseJson = json::parse(response);
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data")) {
            articles = responseJson["data"].get<std::vector<json>>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting trending articles: " << e.what() << std::endl;
    }
    return articles;
}

bool PersonalizationService::trackArticleView(int articleId) {
    try {
        json body = {
            {"user_id", session.getUserId()},
            {"article_id", articleId}
        };
        
        std::string response = client.post("/articles/track-view", body.dump());
        auto responseJson = json::parse(response);
        
        return responseJson.contains("success") && responseJson["success"].get<bool>();
    } catch (const std::exception& e) {
        std::cerr << "Error tracking article view: " << e.what() << std::endl;
        return false;
    }
}

bool PersonalizationService::trackArticleInteraction(int articleId, const std::string& interactionType) {
    try {
        json body = {
            {"user_id", session.getUserId()},
            {"article_id", articleId},
            {"interaction_type", interactionType}
        };
        
        std::string response = client.post("/articles/track-interaction", body.dump());
        auto responseJson = json::parse(response);
        
        return responseJson.contains("success") && responseJson["success"].get<bool>();
    } catch (const std::exception& e) {
        std::cerr << "Error tracking article interaction: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> PersonalizationService::getUserInterests() {
    std::vector<std::string> interests;
    try {
        std::string endpoint = "/user/interests?user_id=" + std::to_string(session.getUserId());
        std::string response = client.get(endpoint);
        
        auto responseJson = json::parse(response);
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data") && responseJson["data"].contains("interests")) {
            interests = responseJson["data"]["interests"].get<std::vector<std::string>>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting user interests: " << e.what() << std::endl;
    }
    return interests;
}

std::vector<std::string> PersonalizationService::getRecommendedCategories() {
    std::vector<std::string> categories;
    try {
        std::string endpoint = "/user/recommended-categories?user_id=" + std::to_string(session.getUserId());
        std::string response = client.get(endpoint);
        
        auto responseJson = json::parse(response);
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data") && responseJson["data"].contains("recommended_categories")) {
            categories = responseJson["data"]["recommended_categories"].get<std::vector<std::string>>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting recommended categories: " << e.what() << std::endl;
    }
    return categories;
} 