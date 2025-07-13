#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class UserMenu {
public:
    UserMenu(Client& httpClient, Session& userSession);
    void display();

private:
    Client& httpClient;
    Session& userSession;

    void displayHeadlines();
    void displaySavedArticles();
    void displaySearch();
    void displayNotifications();
    void displayPersonalizedContent();
};
