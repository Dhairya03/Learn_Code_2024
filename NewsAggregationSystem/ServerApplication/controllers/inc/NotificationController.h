#ifndef NOTIFICATION_CONTROLLER_H
#define NOTIFICATION_CONTROLLER_H

#include <memory>
#include "lib/crow/crow_all.h"
#include "database/inc/DBConnection.h"

class NotificationController {
public:
    static crow::response getNotifications(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response markNotificationAsRead(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response deleteNotification(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getUserNotificationSettings(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response updateCategorySettings(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response updateKeywords(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
};

#endif 