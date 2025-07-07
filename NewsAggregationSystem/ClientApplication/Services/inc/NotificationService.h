#pragma once
#include "../../Core/inc/Client.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class NotificationService {
public:
    NotificationService(Client& client);
    
    // Get user notifications
    std::vector<json> getNotifications(int userId);
    
    // Mark notification as read
    bool markNotificationAsRead(int notificationId);
    
    // Delete notification
    bool deleteNotification(int notificationId);
    
    // Get user notification settings
    json getUserNotificationSettings(int userId);
    
    // Update category settings
    bool updateCategorySettings(int userId, int categoryId, bool enabled);
    
    // Update keywords
    bool updateKeywords(int userId, const std::string& keywords);

private:
    Client& client;
};

