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
