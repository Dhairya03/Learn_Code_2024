#include "../../Services/inc/SavedArticleService.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

SavedArticleService::SavedArticleService(Client& c, Session& s)
    : client(c), session(s) {}

void SavedArticleService::saveArticle(int articleId) {
    json body = {
        {"user_id", session.getUserId()},
        {"article_id", articleId}
    };
    std::string res = client.post("/user/articles/save", body.dump());
    std::cout << "Server: " << res << "\n";
}

void SavedArticleService::deleteArticle(int articleId) {
    json body = {
        {"user_id", session.getUserId()},
        {"article_id", articleId}
    };
    std::string res = client.post("/user/articles/delete", body.dump());
    std::cout << "Server: " << res << "\n";
}
