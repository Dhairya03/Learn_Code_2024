#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"
#include "../../Services/inc/NotificationService.h"

class ConfigureNotificationMenu {
public:
    ConfigureNotificationMenu(Client& client, Session& session);
    void display();

private:
    Client& client;
    Session& session;
    NotificationService notificationService;

    void toggleCategory(int categoryId);
    void configureKeywords();
};
