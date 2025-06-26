#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class SavedMenu {
public:
    SavedMenu(Client& client, Session& session);
    void display();

private:
    Client& client;
    Session& session;

    void showSavedArticles();
    void deleteSavedArticle();
};
