#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"
#include "../../Services/inc/PersonalizationService.h"
#include <iomanip>

class PersonalizedMenu {
public:
    PersonalizedMenu(Client& httpClient, Session& userSession);
    void display();

private:
    Client& httpClient;
    Session& userSession;
    PersonalizationService personalizationService;

    void displayPersonalizedArticles();
    void displayRecommendedArticles();
    void displayTrendingArticles();
    void displayUserInsights();
    void displayArticleTable(const std::vector<nlohmann::json>& articles, const std::string& title);
    void handleArticleInteraction(const nlohmann::json& article);
}; 