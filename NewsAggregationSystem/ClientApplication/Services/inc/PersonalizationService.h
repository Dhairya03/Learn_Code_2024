#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class PersonalizationService {
public:
    PersonalizationService(Client& client, Session& session);
    
    std::vector<json> getPersonalizedArticles(int limit = 20);
    std::vector<json> getRecommendedArticles(int limit = 10);
    std::vector<json> getTrendingArticles(int limit = 10);
    
    bool trackArticleView(int articleId);
    bool trackArticleInteraction(int articleId, const std::string& interactionType);
    
    std::vector<std::string> getUserInterests();
    std::vector<std::string> getRecommendedCategories();

private:
    Client& client;
    Session& session;
}; 