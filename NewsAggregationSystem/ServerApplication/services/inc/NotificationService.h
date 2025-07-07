#ifndef NOTIFICATION_SERVICE_H
#define NOTIFICATION_SERVICE_H

#include <memory>
#include <vector>
#include <string>
#include "database/inc/DBConnection.h"
#include "models/inc/Notification.h"
#include "models/inc/UserNotificationSettings.h"
#include "models/inc/NewsArticle.h"
#include "services/inc/EmailService.h"

class NotificationService {
public:
    explicit NotificationService(std::shared_ptr<DBConnection> dbConn);
    
    // Notification management
    std::vector<Notification> getUserNotifications(int userId);
    bool markNotificationAsRead(int notificationId);
    bool deleteNotification(int notificationId);
    
    // User notification settings
    UserNotificationSettings getUserNotificationSettings(int userId);
    bool updateCategorySettings(int userId, int categoryId, bool enabled);
    bool updateKeywords(int userId, const std::string& keywords);
    bool createUserNotificationSettings(int userId, const std::string& email);
    
    // Notification generation
    void processNewsArticleForNotifications(const NewsArticle& article);
    void sendCategoryNotification(const std::string& categoryName, const NewsArticle& article);
    void sendKeywordNotification(const NewsArticle& article);
    
    // Email notifications
    void sendEmailNotifications(const std::vector<int>& userIds, const NewsArticle& article, const std::string& categoryName);

private:
    std::shared_ptr<DBConnection> dbConn;
    std::unique_ptr<EmailService> emailService;
    
    std::vector<std::string> getUserEmails(const std::vector<int>& userIds);
    std::string getCategoryName(int categoryId);
};

#endif 