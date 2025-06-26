#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class NotificationMenu {
public:
    NotificationMenu(Client& client, Session& session);
    void display();

private:
    Client& client;
    Session& session;

    void viewNotifications();
    void configureNotifications();
};
