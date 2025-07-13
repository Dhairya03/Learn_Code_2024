#pragma once
#include "../../Core/inc/Client.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class NotificationService {
public:
    NotificationService(Client& client);
    
    std::vector<json> getNotifications(int userId);
    
    bool markNotificationAsRead(int notificationId);
    
    bool deleteNotification(int notificationId);
    
    json getUserNotificationSettings(int userId);
    
    bool updateCategorySettings(int userId, int categoryId, bool enabled);
    bool updateCategorySettings(int userId, const std::string& categoryName, bool enabled);
    
    bool updateKeywords(int userId, const std::string& keywords);

private:
    Client& client;
};

