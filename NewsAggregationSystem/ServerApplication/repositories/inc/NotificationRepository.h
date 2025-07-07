#ifndef NOTIFICATION_REPOSITORY_H
#define NOTIFICATION_REPOSITORY_H

#include <memory>
#include <vector>
#include <string>
#include "database/inc/DBConnection.h"
#include "models/inc/Notification.h"
#include "models/inc/UserNotificationSettings.h"

class NotificationRepository {
public:
    explicit NotificationRepository(std::shared_ptr<DBConnection> dbConn);
    
    // Notification operations
    bool createNotification(const Notification& notification);
    std::vector<Notification> getNotificationsByUserId(int userId);
    bool markNotificationAsRead(int notificationId);
    bool deleteNotification(int notificationId);
    
    // User notification settings operations
    bool createUserNotificationSettings(const UserNotificationSettings& settings);
    UserNotificationSettings getUserNotificationSettings(int userId);
    bool updateUserNotificationSettings(const UserNotificationSettings& settings);
    bool updateCategorySettings(int userId, int categoryId, bool enabled);
    bool updateKeywords(int userId, const std::string& keywords);
    
    // Get users to notify for specific categories/keywords
    std::vector<int> getUsersToNotifyForCategory(const std::string& categoryName);
    std::vector<int> getUsersToNotifyForKeywords(const std::string& articleTitle, const std::string& articleDescription);

private:
    std::shared_ptr<DBConnection> dbConn;
    void createTablesIfNotExist();
};

#endif 