#include "controllers/inc/NotificationController.h"
#include "services/inc/NotificationService.h"
#include "lib/json/json.hpp"
#include <iostream>

using json = nlohmann::json;

crow::response NotificationController::getNotifications(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[NotificationController] getNotifications called" << std::endl;
    try {
        auto userId = req.url_params.get("user_id");
        if (!userId) {
            json response = {{"success", false}, {"message", "Missing user_id parameter"}};
            return crow::response(400, response.dump());
        }
        NotificationService service(dbConn);
        auto notifications = service.getUserNotifications(std::stoi(userId));
        json result = json::array();
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
            result.push_back(notificationJson);
        }
        json response = {{"success", true}, {"data", result}};
        std::cout << "[NotificationController] getNotifications success" << std::endl;
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[NotificationController] getNotifications error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}

crow::response NotificationController::markNotificationAsRead(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[NotificationController] markNotificationAsRead called" << std::endl;
    try {
        auto notificationId = req.url_params.get("notification_id");
        if (!notificationId) {
            json response = {{"success", false}, {"message", "Missing notification_id parameter"}};
            return crow::response(400, response.dump());
        }
        NotificationService service(dbConn);
        bool success = service.markNotificationAsRead(std::stoi(notificationId));
        if (success) {
            json response = {{"success", true}, {"message", "Notification marked as read"}};
            std::cout << "[NotificationController] markNotificationAsRead success" << std::endl;
            return crow::response(200, response.dump());
        } else {
            json response = {{"success", false}, {"message", "Notification not found"}};
            std::cerr << "[NotificationController] markNotificationAsRead not found" << std::endl;
            return crow::response(404, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[NotificationController] markNotificationAsRead error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}

crow::response NotificationController::deleteNotification(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[NotificationController] deleteNotification called" << std::endl;
    try {
        auto notificationId = req.url_params.get("notification_id");
        if (!notificationId) {
            json response = {{"success", false}, {"message", "Missing notification_id parameter"}};
            return crow::response(400, response.dump());
        }
        NotificationService service(dbConn);
        bool success = service.deleteNotification(std::stoi(notificationId));
        if (success) {
            json response = {{"success", true}, {"message", "Notification deleted"}};
            std::cout << "[NotificationController] deleteNotification success" << std::endl;
            return crow::response(200, response.dump());
        } else {
            json response = {{"success", false}, {"message", "Notification not found"}};
            std::cerr << "[NotificationController] deleteNotification not found" << std::endl;
            return crow::response(404, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[NotificationController] deleteNotification error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}

crow::response NotificationController::getUserNotificationSettings(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[NotificationController] getUserNotificationSettings called" << std::endl;
    try {
        auto userId = req.url_params.get("user_id");
        if (!userId) {
            json response = {{"success", false}, {"message", "Missing user_id parameter"}};
            return crow::response(400, response.dump());
        }
        NotificationService service(dbConn);
        auto settings = service.getUserNotificationSettings(std::stoi(userId));
        json result = {
            {"id", settings.id},
            {"user_id", settings.userId},
            {"business_enabled", settings.businessEnabled},
            {"entertainment_enabled", settings.entertainmentEnabled},
            {"sports_enabled", settings.sportsEnabled},
            {"technology_enabled", settings.technologyEnabled},
            {"keywords_enabled", settings.keywordsEnabled},
            {"keywords", settings.keywords},
            {"email", settings.email}
        };
        json response = {{"success", true}, {"data", result}};
        std::cout << "[NotificationController] getUserNotificationSettings success" << std::endl;
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[NotificationController] getUserNotificationSettings error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}

crow::response NotificationController::updateCategorySettings(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[NotificationController] updateCategorySettings called" << std::endl;
    try {
        auto body = json::parse(req.body);
        if (!body.contains("user_id") || !body.contains("category_id") || !body.contains("enabled")) {
            json response = {{"success", false}, {"message", "Missing required fields: user_id, category_id, enabled"}};
            return crow::response(400, response.dump());
        }
        int userId = body["user_id"];
        int categoryId = body["category_id"];
        bool enabled = body["enabled"];
        NotificationService service(dbConn);
        bool success = service.updateCategorySettings(userId, categoryId, enabled);
        if (success) {
            json response = {{"success", true}, {"message", "Category settings updated"}};
            std::cout << "[NotificationController] updateCategorySettings success" << std::endl;
            return crow::response(200, response.dump());
        } else {
            json response = {{"success", false}, {"message", "Failed to update category settings"}};
            std::cerr << "[NotificationController] updateCategorySettings failed" << std::endl;
            return crow::response(400, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[NotificationController] updateCategorySettings error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}

crow::response NotificationController::updateKeywords(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[NotificationController] updateKeywords called" << std::endl;
    try {
        auto body = json::parse(req.body);
        if (!body.contains("user_id") || !body.contains("keywords")) {
            json response = {{"success", false}, {"message", "Missing required fields: user_id, keywords"}};
            return crow::response(400, response.dump());
        }
        int userId = body["user_id"];
        std::string keywords = body["keywords"];
        NotificationService service(dbConn);
        bool success = service.updateKeywords(userId, keywords);
        if (success) {
            json response = {{"success", true}, {"message", "Keywords updated"}};
            std::cout << "[NotificationController] updateKeywords success" << std::endl;
            return crow::response(200, response.dump());
        } else {
            json response = {{"success", false}, {"message", "Failed to update keywords"}};
            std::cerr << "[NotificationController] updateKeywords failed" << std::endl;
            return crow::response(400, response.dump());
        }
    } catch (const std::exception& e) {
        std::cerr << "[NotificationController] updateKeywords error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
} 