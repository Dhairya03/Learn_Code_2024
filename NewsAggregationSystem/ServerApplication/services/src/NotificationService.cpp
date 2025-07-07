#include "services/inc/NotificationService.h"
#include "repositories/inc/NotificationRepository.h"
#include "repositories/inc/UserRepository.h"
#include <iostream>
#include <sstream>

NotificationService::NotificationService(std::shared_ptr<DBConnection> dbConn) 
    : dbConn(std::move(dbConn)), emailService(std::make_unique<EmailService>()) {
}

std::vector<Notification> NotificationService::getUserNotifications(int userId) {
    std::cout << "[NotificationService] getUserNotifications called" << std::endl;
    try {
        NotificationRepository repo(dbConn);
        auto result = repo.getNotificationsByUserId(userId);
        std::cout << "[NotificationService] getUserNotifications success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationService] getUserNotifications error: " << e.what() << std::endl;
        return {};
    }
}

bool NotificationService::markNotificationAsRead(int notificationId) {
    std::cout << "[NotificationService] markNotificationAsRead called" << std::endl;
    try {
        NotificationRepository repo(dbConn);
        bool result = repo.markNotificationAsRead(notificationId);
        std::cout << "[NotificationService] markNotificationAsRead success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationService] markNotificationAsRead error: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationService::deleteNotification(int notificationId) {
    std::cout << "[NotificationService] deleteNotification called" << std::endl;
    try {
        NotificationRepository repo(dbConn);
        bool result = repo.deleteNotification(notificationId);
        std::cout << "[NotificationService] deleteNotification success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationService] deleteNotification error: " << e.what() << std::endl;
        return false;
    }
}

UserNotificationSettings NotificationService::getUserNotificationSettings(int userId) {
    std::cout << "[NotificationService] getUserNotificationSettings called" << std::endl;
    try {
        NotificationRepository repo(dbConn);
        auto result = repo.getUserNotificationSettings(userId);
        std::cout << "[NotificationService] getUserNotificationSettings success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationService] getUserNotificationSettings error: " << e.what() << std::endl;
        return {};
    }
}

bool NotificationService::updateCategorySettings(int userId, int categoryId, bool enabled) {
    std::cout << "[NotificationService] updateCategorySettings called" << std::endl;
    try {
        NotificationRepository repo(dbConn);
        bool result = repo.updateCategorySettings(userId, categoryId, enabled);
        std::cout << "[NotificationService] updateCategorySettings success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationService] updateCategorySettings error: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationService::updateKeywords(int userId, const std::string& keywords) {
    std::cout << "[NotificationService] updateKeywords called" << std::endl;
    try {
        NotificationRepository repo(dbConn);
        bool result = repo.updateKeywords(userId, keywords);
        std::cout << "[NotificationService] updateKeywords success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationService] updateKeywords error: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationService::createUserNotificationSettings(int userId, const std::string& email) {
    std::cout << "[NotificationService] createUserNotificationSettings called" << std::endl;
    try {
        NotificationRepository repo(dbConn);
        UserNotificationSettings settings;
        settings.userId = userId;
        settings.email = email;
        settings.businessEnabled = false;
        settings.entertainmentEnabled = false;
        settings.sportsEnabled = false;
        settings.technologyEnabled = false;
        settings.keywordsEnabled = false;
        bool result = repo.createUserNotificationSettings(settings);
        std::cout << "[NotificationService] createUserNotificationSettings success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationService] createUserNotificationSettings error: " << e.what() << std::endl;
        return false;
    }
}

void NotificationService::processNewsArticleForNotifications(const NewsArticle& article) {
    std::cout << "[NotificationService] processNewsArticleForNotifications called" << std::endl;
    try {
        std::string categoryName = getCategoryName(article.categoryId);
        if (!categoryName.empty()) {
            sendCategoryNotification(categoryName, article);
        }
        sendKeywordNotification(article);
        std::cout << "[NotificationService] processNewsArticleForNotifications success" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationService] processNewsArticleForNotifications error: " << e.what() << std::endl;
    }
}

void NotificationService::sendCategoryNotification(const std::string& categoryName, const NewsArticle& article) {
    std::cout << "[NotificationService] sendCategoryNotification called" << std::endl;
    try {
        NotificationRepository repo(dbConn);
        auto userIds = repo.getUsersToNotifyForCategory(categoryName);
        if (!userIds.empty()) {
            std::cout << "[NotificationService] Sending category notifications to " << userIds.size() << " users for category: " << categoryName << std::endl;
            for (int userId : userIds) {
                Notification notification;
                notification.userId = userId;
                notification.type = "category";
                notification.categoryName = categoryName;
                notification.articleId = article.id;
                notification.message = "New " + categoryName + " news: " + article.title;
                repo.createNotification(notification);
            }
            sendEmailNotifications(userIds, article, categoryName);
        }
        std::cout << "[NotificationService] sendCategoryNotification success" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationService] sendCategoryNotification error: " << e.what() << std::endl;
    }
}

void NotificationService::sendKeywordNotification(const NewsArticle& article) {
    std::cout << "[NotificationService] sendKeywordNotification called" << std::endl;
    try {
        NotificationRepository repo(dbConn);
        auto userIds = repo.getUsersToNotifyForKeywords(article.title, article.description);
        if (!userIds.empty()) {
            std::cout << "[NotificationService] Sending keyword notifications to " << userIds.size() << " users" << std::endl;
            for (int userId : userIds) {
                Notification notification;
                notification.userId = userId;
                notification.type = "keyword";
                notification.articleId = article.id;
                notification.message = "Keyword match: " + article.title;
                repo.createNotification(notification);
            }
            std::string categoryName = getCategoryName(article.categoryId);
            sendEmailNotifications(userIds, article, categoryName);
        }
        std::cout << "[NotificationService] sendKeywordNotification success" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationService] sendKeywordNotification error: " << e.what() << std::endl;
    }
}

void NotificationService::sendEmailNotifications(const std::vector<int>& userIds, const NewsArticle& article, const std::string& categoryName) {
    std::cout << "[NotificationService] sendEmailNotifications called" << std::endl;
    try {
        auto emails = getUserEmails(userIds);
        if (!emails.empty()) {
            std::cout << "[NotificationService] Sending email notifications to " << emails.size() << " users" << std::endl;
            for (const auto& email : emails) {
                emailService->sendNewsNotification(email, article.title, article.description, article.url, categoryName);
            }
        }
        std::cout << "[NotificationService] sendEmailNotifications success" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationService] sendEmailNotifications error: " << e.what() << std::endl;
    }
}

std::vector<std::string> NotificationService::getUserEmails(const std::vector<int>& userIds) {
    std::cout << "[NotificationService] getUserEmails called" << std::endl;
    std::vector<std::string> emails;
    try {
        UserRepository userRepo(dbConn);
        for (int userId : userIds) {
            auto user = userRepo.getUserById(userId);
            if (!user.email.empty()) {
                emails.push_back(user.email);
            }
        }
        std::cout << "[NotificationService] getUserEmails success" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationService] getUserEmails error: " << e.what() << std::endl;
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