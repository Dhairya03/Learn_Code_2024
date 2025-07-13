#include "../../Services/inc/SavedArticleService.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

SavedArticleService::SavedArticleService(Client& httpClient, Session& userSession)
    : httpClient(httpClient), userSession(userSession) {}

void SavedArticleService::saveArticle(int articleId) {
    json body = {
        {"user_id", userSession.getUserId()},
        {"article_id", articleId}
    };
    std::string requestBody = body.dump();
    std::cout << "[DEBUG] Save request body: " << requestBody << std::endl;
    std::string res = httpClient.post("/user/articles/save", requestBody);
    std::cout << "[DEBUG] Save response: " << res << std::endl;
}

void SavedArticleService::deleteArticle(int articleId) {
    std::cout << "[DEBUG] Deleting article ID: " << articleId << std::endl;
    std::string endpoint = "/user/articles/" + std::to_string(articleId) + "?user_id=" + std::to_string(userSession.getUserId());
    std::cout << "[DEBUG] Delete endpoint: " << endpoint << std::endl;
    
    std::string res = httpClient.delete_(endpoint);
    std::cout << "[DEBUG] Delete response: " << res << std::endl;
}

std::vector<nlohmann::json> SavedArticleService::getSavedArticles() {
    std::vector<nlohmann::json> articles;
    try {
        std::string endpoint = "/user/articles?user_id=" + std::to_string(userSession.getUserId());
        std::string response = httpClient.get(endpoint);
        
        if (!response.empty()) {
            auto responseJson = json::parse(response);
            if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
                responseJson.contains("data")) {
                auto data = responseJson["data"];
                if (data.is_array()) {
                    for (const auto& article : data) {
                        articles.push_back(article);
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting saved articles: " << e.what() << std::endl;
    }
    return articles;
}
