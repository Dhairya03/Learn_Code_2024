#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"
#include "../../Services/inc/NotificationService.h"

class NotificationMenu {
public:
    NotificationMenu(Client& httpClient, Session& userSession);
    void display();

private:
    Client& httpClient;
    Session& userSession;
    NotificationService notificationService;

    void displayNotificationsTable();
    void displayConfigureNotificationsMenu();
};
