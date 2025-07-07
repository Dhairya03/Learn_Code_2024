#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class SavedMenu {
public:
    SavedMenu(Client& httpClient, Session& userSession);
    void display();

private:
    Client& httpClient;
    Session& userSession;

    void displaySavedArticlesTable();
    void removeSavedArticle();
};
