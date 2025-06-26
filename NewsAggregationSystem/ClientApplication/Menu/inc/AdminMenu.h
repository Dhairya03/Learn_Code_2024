#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class AdminMenu {
public:
    AdminMenu(Client& client, Session& session);
    void display();

private:
    Client& client;
    Session& session;

    void viewServerStatus();
    void viewServerDetails();
    void updateServerDetails();
    void addCategory();
};
