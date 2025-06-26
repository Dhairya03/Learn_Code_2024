#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class SearchMenu {
public:
    SearchMenu(Client& client, Session& session);
    void display();

private:
    Client& client;
    Session& session;

    void performSearch();
};
