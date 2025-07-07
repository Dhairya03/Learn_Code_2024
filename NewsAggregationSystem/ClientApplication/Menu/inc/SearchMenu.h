#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"
#include "../../Services/inc/ArticleService.h"

class SearchMenu {
public:
    SearchMenu(Client& httpClient, Session& userSession);
    void display();

private:
    Client& httpClient;
    Session& userSession;
    ArticleService articleService;

    void displaySearchResults();
};
