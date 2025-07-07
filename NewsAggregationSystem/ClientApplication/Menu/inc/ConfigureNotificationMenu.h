#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"
#include "../../Services/inc/NotificationService.h"

class ConfigureNotificationMenu {
public:
    ConfigureNotificationMenu(Client& httpClient, Session& userSession);
    void display();

private:
    Client& httpClient;
    Session& userSession;
    NotificationService notificationService;

    void toggleCategorySetting(int categoryId);
    void configureNotificationKeywords();
};
