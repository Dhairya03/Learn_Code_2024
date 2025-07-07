#include "services/inc/NotificationService.h"
#include "repositories/inc/NotificationRepository.h"
#include "repositories/inc/UserRepository.h"
#include <iostream>
#include <sstream>

NotificationService::NotificationService(std::shared_ptr<DBConnection> dbConn) 
    : dbConn(std::move(dbConn)), emailService(std::make_unique<EmailService>()) {
}

std::vector<Notification> NotificationService::getUserNotifications(int userId) {
    NotificationRepository repo(dbConn);
    return repo.getNotificationsByUserId(userId);
}

bool NotificationService::markNotificationAsRead(int notificationId) {
    NotificationRepository repo(dbConn);
    return repo.markNotificationAsRead(notificationId);
}

bool NotificationService::deleteNotification(int notificationId) {
    NotificationRepository repo(dbConn);
    return repo.deleteNotification(notificationId);
}

UserNotificationSettings NotificationService::getUserNotificationSettings(int userId) {
    NotificationRepository repo(dbConn);
    return repo.getUserNotificationSettings(userId);
}

bool NotificationService::updateCategorySettings(int userId, int categoryId, bool enabled) {
    NotificationRepository repo(dbConn);
    return repo.updateCategorySettings(userId, categoryId, enabled);
}

bool NotificationService::updateKeywords(int userId, const std::string& keywords) {
    NotificationRepository repo(dbConn);
    return repo.updateKeywords(userId, keywords);
}

bool NotificationService::createUserNotificationSettings(int userId, const std::string& email) {
    NotificationRepository repo(dbConn);
    
    UserNotificationSettings settings;
    settings.userId = userId;
    settings.email = email;
    settings.businessEnabled = false;
    settings.entertainmentEnabled = false;
    settings.sportsEnabled = false;
    settings.technologyEnabled = false;
    settings.keywordsEnabled = false;
    
    return repo.createUserNotificationSettings(settings);
}

void NotificationService::processNewsArticleForNotifications(const NewsArticle& article) {
    std::cout << "[NOTIFICATION SERVICE] Processing article for notifications: " << article.title << std::endl;
    
    // Send category-based notifications
    std::string categoryName = getCategoryName(article.categoryId);
    if (!categoryName.empty()) {
        sendCategoryNotification(categoryName, article);
    }
    
    // Send keyword-based notifications
    sendKeywordNotification(article);
}

void NotificationService::sendCategoryNotification(const std::string& categoryName, const NewsArticle& article) {
    NotificationRepository repo(dbConn);
    auto userIds = repo.getUsersToNotifyForCategory(categoryName);
    
    if (!userIds.empty()) {
        std::cout << "[NOTIFICATION SERVICE] Sending category notifications to " << userIds.size() << " users for category: " << categoryName << std::endl;
        
        // Create notifications in database
        for (int userId : userIds) {
            Notification notification;
            notification.userId = userId;
            notification.type = "category";
            notification.categoryName = categoryName;
            notification.articleId = article.id;
            notification.message = "New " + categoryName + " news: " + article.title;
            
            repo.createNotification(notification);
        }
        
        // Send email notifications
        sendEmailNotifications(userIds, article, categoryName);
    }
}

void NotificationService::sendKeywordNotification(const NewsArticle& article) {
    NotificationRepository repo(dbConn);
    auto userIds = repo.getUsersToNotifyForKeywords(article.title, article.description);
    
    if (!userIds.empty()) {
        std::cout << "[NOTIFICATION SERVICE] Sending keyword notifications to " << userIds.size() << " users" << std::endl;
        
        // Create notifications in database
        for (int userId : userIds) {
            Notification notification;
            notification.userId = userId;
            notification.type = "keyword";
            notification.articleId = article.id;
            notification.message = "Keyword match: " + article.title;
            
            repo.createNotification(notification);
        }
        
        // Send email notifications
        std::string categoryName = getCategoryName(article.categoryId);
        sendEmailNotifications(userIds, article, categoryName);
    }
}

void NotificationService::sendEmailNotifications(const std::vector<int>& userIds, const NewsArticle& article, const std::string& categoryName) {
    auto emails = getUserEmails(userIds);
    
    if (!emails.empty()) {
        std::cout << "[NOTIFICATION SERVICE] Sending email notifications to " << emails.size() << " users" << std::endl;
        
        for (const auto& email : emails) {
            emailService->sendNewsNotification(email, article.title, article.description, article.url, categoryName);
        }
    }
}

std::vector<std::string> NotificationService::getUserEmails(const std::vector<int>& userIds) {
    std::vector<std::string> emails;
    UserRepository userRepo(dbConn);
    
    for (int userId : userIds) {
        auto user = userRepo.getUserById(userId);
        if (!user.email.empty()) {
            emails.push_back(user.email);
        }
    }
    
    return emails;
}

std::string NotificationService::getCategoryName(int categoryId) {
    switch (categoryId) {
        case 1: return "business";
        case 2: return "entertainment";
        case 3: return "sports";
        case 4: return "technology";
        default: return "";
    }
} 