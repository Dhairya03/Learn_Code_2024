#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class UserMenu {
public:
    UserMenu(Client& client, Session& session);
    void display();

private:
    Client& client;
    Session& session;

    void showHeadlines();
    void showSavedArticles();
    void showSearch();
    void showNotifications();
};
