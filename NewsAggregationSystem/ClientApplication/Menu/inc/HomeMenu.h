#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class HomeMenu {
public:
    HomeMenu(Client& client, Session& session);
    void display();

private:
    Client& client;
    Session& session;

    void login();
    void signup();
    void exitApp();
};
