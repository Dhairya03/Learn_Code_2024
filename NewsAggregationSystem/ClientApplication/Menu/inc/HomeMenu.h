#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class HomeMenu {
public:
    HomeMenu(Client& httpClient, Session& userSession);
    void display();

private:
    Client& httpClient;
    Session& userSession;

    void displayLogin();
    void displaySignup();
    void exitApplication();
};
