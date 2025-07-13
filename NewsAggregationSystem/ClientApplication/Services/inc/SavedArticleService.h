#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"
#include <vector>
#include <nlohmann/json.hpp>

class SavedArticleService {
public:
    SavedArticleService(Client& httpClient, Session& userSession);
    void saveArticle(int articleId);
    void deleteArticle(int articleId);
    std::vector<nlohmann::json> getSavedArticles();

private:
    Client& httpClient;
    Session& userSession;
};
