#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class ConfigureNotificationMenu {
public:
    ConfigureNotificationMenu(Client& client, Session& session);
    void display();

private:
    Client& client;
    Session& session;

    void toggleCategory(int categoryId);
    void configureKeywords();
};
