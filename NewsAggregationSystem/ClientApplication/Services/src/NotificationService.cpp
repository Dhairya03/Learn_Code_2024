#include "../inc/NotificationService.h"
#include <iostream>

NotificationService::NotificationService(Client& client) : client(client) {}

std::vector<json> NotificationService::getNotifications(int userId) {
    std::vector<json> notifications;
    
    try {
        std::string endpoint = "/notifications?user_id=" + std::to_string(userId);
        std::string response = client.get(endpoint);
        
        if (!response.empty()) {
            auto notificationsJson = json::parse(response);
            if (notificationsJson.is_array()) {
                for (const auto& notification : notificationsJson) {
                    notifications.push_back(notification);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting notifications: " << e.what() << std::endl;
    }
    
    return notifications;
}

bool NotificationService::markNotificationAsRead(int notificationId) {
    try {
        std::string endpoint = "/notifications/read?notification_id=" + std::to_string(notificationId);
        std::string response = client.post(endpoint, "");
        
        auto responseJson = json::parse(response);
        return responseJson.contains("message");
    } catch (const std::exception& e) {
        std::cerr << "Error marking notification as read: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationService::deleteNotification(int notificationId) {
    try {
        std::string endpoint = "/notifications/delete?notification_id=" + std::to_string(notificationId);
        std::string response = client.post(endpoint, "");
        
        auto responseJson = json::parse(response);
        return responseJson.contains("message");
    } catch (const std::exception& e) {
        std::cerr << "Error deleting notification: " << e.what() << std::endl;
        return false;
    }
}

json NotificationService::getUserNotificationSettings(int userId) {
    json settings;
    
    try {
        std::string endpoint = "/notifications/settings?user_id=" + std::to_string(userId);
        std::string response = client.get(endpoint);
        
        if (!response.empty()) {
            settings = json::parse(response);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting user notification settings: " << e.what() << std::endl;
    }
    
    return settings;
}

bool NotificationService::updateCategorySettings(int userId, int categoryId, bool enabled) {
    try {
        json body = {
            {"user_id", userId},
            {"category_id", categoryId},
            {"enabled", enabled}
        };
        
        std::string response = client.post("/notifications/configure/category", body.dump());
        
        auto responseJson = json::parse(response);
        return responseJson.contains("message");
    } catch (const std::exception& e) {
        std::cerr << "Error updating category settings: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationService::updateCategorySettings(int userId, const std::string& categoryName, bool enabled) {
    try {
        json body = {
            {"user_id", userId},
            {"category_name", categoryName},
            {"enabled", enabled}
        };
        
        std::string response = client.post("/notifications/configure/category", body.dump());
        
        auto responseJson = json::parse(response);
        return responseJson.contains("message");
    } catch (const std::exception& e) {
        std::cerr << "Error updating category settings: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationService::updateKeywords(int userId, const std::string& keywords) {
    try {
        json body = {
            {"user_id", userId},
            {"keywords", keywords}
        };
        
        std::string response = client.post("/notifications/configure/keywords", body.dump());
        
        auto responseJson = json::parse(response);
        return responseJson.contains("message");
    } catch (const std::exception& e) {
        std::cerr << "Error updating keywords: " << e.what() << std::endl;
        return false;
    }
}
