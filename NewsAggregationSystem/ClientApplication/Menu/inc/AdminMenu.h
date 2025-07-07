#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class AdminMenu {
public:
    AdminMenu(Client& httpClient, Session& userSession);
    void display();

private:
    Client& httpClient;
    Session& userSession;

    void displayServerStatus();
    void displayServerDetails();
    void updateServerApiKey();
    void addNewsCategory();
    void displayReportedArticles();
    void toggleCategoryVisibility();
    void displayNotifications();
};
