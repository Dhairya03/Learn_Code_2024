#include "controllers/inc/NotificationController.h"
#include "services/inc/NotificationService.h"
#include "lib/json/json.hpp"
#include <iostream>

using json = nlohmann::json;

crow::response NotificationController::getNotifications(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    try {
        auto userId = req.url_params.get("user_id");
        if (!userId) {
            return crow::response(400, "Missing user_id parameter");
        }
        
        NotificationService service(dbConn);
        auto notifications = service.getUserNotifications(std::stoi(userId));
        
        json response;
        for (const auto& notification : notifications) {
            json notificationJson;
            notificationJson["id"] = notification.id;
            notificationJson["user_id"] = notification.userId;
            notificationJson["message"] = notification.message;
            notificationJson["type"] = notification.type;
            notificationJson["timestamp"] = notification.timestamp;
            notificationJson["is_read"] = notification.isRead;
            notificationJson["article_id"] = notification.articleId;
            notificationJson["category_name"] = notification.categoryName;
            notificationJson["keywords"] = notification.keywords;
            
            response.push_back(notificationJson);
        }
        
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "Error getting notifications: " << e.what() << std::endl;
        return crow::response(500, "Internal server error");
    }
}

crow::response NotificationController::markNotificationAsRead(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    try {
        auto notificationId = req.url_params.get("notification_id");
        if (!notificationId) {
            return crow::response(400, "Missing notification_id parameter");
        }
        
        NotificationService service(dbConn);
        bool success = service.markNotificationAsRead(std::stoi(notificationId));
        
        if (success) {
            return crow::response(200, "{\"message\": \"Notification marked as read\"}");
        } else {
            return crow::response(404, "{\"error\": \"Notification not found\"}");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error marking notification as read: " << e.what() << std::endl;
        return crow::response(500, "Internal server error");
    }
}

crow::response NotificationController::deleteNotification(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    try {
        auto notificationId = req.url_params.get("notification_id");
        if (!notificationId) {
            return crow::response(400, "Missing notification_id parameter");
        }
        
        NotificationService service(dbConn);
        bool success = service.deleteNotification(std::stoi(notificationId));
        
        if (success) {
            return crow::response(200, "{\"message\": \"Notification deleted\"}");
        } else {
            return crow::response(404, "{\"error\": \"Notification not found\"}");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error deleting notification: " << e.what() << std::endl;
        return crow::response(500, "Internal server error");
    }
}

crow::response NotificationController::getUserNotificationSettings(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    try {
        auto userId = req.url_params.get("user_id");
        if (!userId) {
            return crow::response(400, "Missing user_id parameter");
        }
        
        NotificationService service(dbConn);
        auto settings = service.getUserNotificationSettings(std::stoi(userId));
        
        json response;
        response["id"] = settings.id;
        response["user_id"] = settings.userId;
        response["business_enabled"] = settings.businessEnabled;
        response["entertainment_enabled"] = settings.entertainmentEnabled;
        response["sports_enabled"] = settings.sportsEnabled;
        response["technology_enabled"] = settings.technologyEnabled;
        response["keywords_enabled"] = settings.keywordsEnabled;
        response["keywords"] = settings.keywords;
        response["email"] = settings.email;
        
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "Error getting user notification settings: " << e.what() << std::endl;
        return crow::response(500, "Internal server error");
    }
}

crow::response NotificationController::updateCategorySettings(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    try {
        auto body = json::parse(req.body);
        
        if (!body.contains("user_id") || !body.contains("category_id") || !body.contains("enabled")) {
            return crow::response(400, "Missing required fields: user_id, category_id, enabled");
        }
        
        int userId = body["user_id"];
        int categoryId = body["category_id"];
        bool enabled = body["enabled"];
        
        NotificationService service(dbConn);
        bool success = service.updateCategorySettings(userId, categoryId, enabled);
        
        if (success) {
            return crow::response(200, "{\"message\": \"Category settings updated\"}");
        } else {
            return crow::response(400, "{\"error\": \"Failed to update category settings\"}");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error updating category settings: " << e.what() << std::endl;
        return crow::response(500, "Internal server error");
    }
}

crow::response NotificationController::updateKeywords(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    try {
        auto body = json::parse(req.body);
        
        if (!body.contains("user_id") || !body.contains("keywords")) {
            return crow::response(400, "Missing required fields: user_id, keywords");
        }
        
        int userId = body["user_id"];
        std::string keywords = body["keywords"];
        
        NotificationService service(dbConn);
        bool success = service.updateKeywords(userId, keywords);
        
        if (success) {
            return crow::response(200, "{\"message\": \"Keywords updated\"}");
        } else {
            return crow::response(400, "{\"error\": \"Failed to update keywords\"}");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error updating keywords: " << e.what() << std::endl;
        return crow::response(500, "Internal server error");
    }
} 