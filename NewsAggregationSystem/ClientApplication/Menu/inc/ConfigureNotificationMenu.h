#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"
#include "../../Services/inc/NotificationService.h"
#include "../../Services/inc/CategoryService.h"

class ConfigureNotificationMenu {
public:
    ConfigureNotificationMenu(Client& httpClient, Session& userSession);
    void display();

private:
    Client& httpClient;
    Session& userSession;
    NotificationService notificationService;
    CategoryService categoryService;

    void toggleCategorySetting(int categoryIndex, const std::vector<std::string>& categories);
    void configureNotificationKeywords();
};
