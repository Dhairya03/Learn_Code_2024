#include "repositories/inc/NotificationRepository.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

NotificationRepository::NotificationRepository(std::shared_ptr<DBConnection> dbConn) 
    : dbConn(std::move(dbConn)) {
    createTablesIfNotExist();
}

void NotificationRepository::createTablesIfNotExist() {
    auto conn = dbConn->getConnection();
    
    std::string createNotificationsTable = R"(
        CREATE TABLE IF NOT EXISTS notifications (
            id INT AUTO_INCREMENT PRIMARY KEY,
            user_id INT NOT NULL,
            message TEXT NOT NULL,
            type VARCHAR(50) NOT NULL,
            timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            is_read BOOLEAN DEFAULT FALSE,
            article_id INT DEFAULT 0,
            category_name VARCHAR(100),
            keywords TEXT,
            FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
        )
    )";
    
    std::string createSettingsTable = R"(
        CREATE TABLE IF NOT EXISTS user_notification_settings (
            id INT AUTO_INCREMENT PRIMARY KEY,
            user_id INT UNIQUE NOT NULL,
            business_enabled BOOLEAN DEFAULT FALSE,
            entertainment_enabled BOOLEAN DEFAULT FALSE,
            sports_enabled BOOLEAN DEFAULT FALSE,
            technology_enabled BOOLEAN DEFAULT FALSE,
            keywords_enabled BOOLEAN DEFAULT FALSE,
            keywords TEXT,
            email VARCHAR(255),
            FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
        )
    )";
    
    try {
        auto stmt1 = conn->createStatement();
        stmt1->execute(createNotificationsTable);
        
        auto stmt2 = conn->createStatement();
        stmt2->execute(createSettingsTable);
    } catch (const sql::SQLException& e) {
        std::cerr << "Error creating notification tables: " << e.what() << std::endl;
    }
}

bool NotificationRepository::createNotification(const Notification& notification) {
    std::cout << "[NotificationRepository] createNotification called" << std::endl;
    auto conn = dbConn->getConnection();
    std::string query = R"(
        INSERT INTO notifications (user_id, message, type, article_id, category_name, keywords)
        VALUES (?, ?, ?, ?, ?, ?)
    )";
    try {
        auto stmt = conn->prepareStatement(query);
        stmt->setInt(1, notification.userId);
        stmt->setString(2, notification.message);
        stmt->setString(3, notification.type);
        stmt->setInt(4, notification.articleId);
        stmt->setString(5, notification.categoryName);
        stmt->setString(6, notification.keywords);
        bool result = stmt->executeUpdate() > 0;
        std::cout << "[NotificationRepository] createNotification success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationRepository] createNotification error: " << e.what() << std::endl;
        return false;
    }
}

std::vector<Notification> NotificationRepository::getNotificationsByUserId(int userId) {
    std::cout << "[NotificationRepository] getNotificationsByUserId called" << std::endl;
    auto conn = dbConn->getConnection();
    std::vector<Notification> notifications;
    std::string query = R"(
        SELECT id, user_id, message, type, timestamp, is_read, article_id, category_name, keywords
        FROM notifications 
        WHERE user_id = ? 
        ORDER BY timestamp DESC
    )";
    try {
        auto stmt = conn->prepareStatement(query);
        stmt->setInt(1, userId);
        auto rs = stmt->executeQuery();
        while (rs->next()) {
            Notification notification;
            notification.id = rs->getInt("id");
            notification.userId = rs->getInt("user_id");
            notification.message = rs->getString("message");
            notification.type = rs->getString("type");
            notification.timestamp = rs->getString("timestamp");
            notification.isRead = rs->getBoolean("is_read");
            notification.articleId = rs->getInt("article_id");
            notification.categoryName = rs->getString("category_name");
            notification.keywords = rs->getString("keywords");
            notifications.push_back(notification);
        }
        std::cout << "[NotificationRepository] getNotificationsByUserId success" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationRepository] getNotificationsByUserId error: " << e.what() << std::endl;
    }
    return notifications;
}

bool NotificationRepository::markNotificationAsRead(int notificationId) {
    std::cout << "[NotificationRepository] markNotificationAsRead called" << std::endl;
    auto conn = dbConn->getConnection();
    std::string query = "UPDATE notifications SET is_read = TRUE WHERE id = ?";
    try {
        auto stmt = conn->prepareStatement(query);
        stmt->setInt(1, notificationId);
        bool result = stmt->executeUpdate() > 0;
        std::cout << "[NotificationRepository] markNotificationAsRead success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationRepository] markNotificationAsRead error: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationRepository::deleteNotification(int notificationId) {
    std::cout << "[NotificationRepository] deleteNotification called" << std::endl;
    auto conn = dbConn->getConnection();
    std::string query = "DELETE FROM notifications WHERE id = ?";
    try {
        auto stmt = conn->prepareStatement(query);
        stmt->setInt(1, notificationId);
        bool result = stmt->executeUpdate() > 0;
        std::cout << "[NotificationRepository] deleteNotification success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NotificationRepository] deleteNotification error: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationRepository::createUserNotificationSettings(const UserNotificationSettings& settings) {
    auto conn = dbConn->getConnection();
    
    std::string query = R"(
        INSERT INTO user_notification_settings 
        (user_id, business_enabled, entertainment_enabled, sports_enabled, technology_enabled, keywords_enabled, keywords, email)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?)
    )";
    
    try {
        auto stmt = conn->prepareStatement(query);
        stmt->setInt(1, settings.userId);
        stmt->setBoolean(2, settings.businessEnabled);
        stmt->setBoolean(3, settings.entertainmentEnabled);
        stmt->setBoolean(4, settings.sportsEnabled);
        stmt->setBoolean(5, settings.technologyEnabled);
        stmt->setBoolean(6, settings.keywordsEnabled);
        stmt->setString(7, settings.keywords);
        stmt->setString(8, settings.email);
        
        return stmt->executeUpdate() > 0;
    } catch (const sql::SQLException& e) {
        std::cerr << "Error creating user notification settings: " << e.what() << std::endl;
        return false;
    }
}

UserNotificationSettings NotificationRepository::getUserNotificationSettings(int userId) {
    auto conn = dbConn->getConnection();
    UserNotificationSettings settings;
    settings.userId = userId;
    
    std::string query = "SELECT * FROM user_notification_settings WHERE user_id = ?";
    
    try {
        auto stmt = conn->prepareStatement(query);
        stmt->setInt(1, userId);
        
        auto rs = stmt->executeQuery();
        if (rs->next()) {
            settings.id = rs->getInt("id");
            settings.businessEnabled = rs->getBoolean("business_enabled");
            settings.entertainmentEnabled = rs->getBoolean("entertainment_enabled");
            settings.sportsEnabled = rs->getBoolean("sports_enabled");
            settings.technologyEnabled = rs->getBoolean("technology_enabled");
            settings.keywordsEnabled = rs->getBoolean("keywords_enabled");
            settings.keywords = rs->getString("keywords");
            settings.email = rs->getString("email");
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "Error getting user notification settings: " << e.what() << std::endl;
    }
    
    return settings;
}

bool NotificationRepository::updateUserNotificationSettings(const UserNotificationSettings& settings) {
    auto conn = dbConn->getConnection();
    
    std::string query = R"(
        UPDATE user_notification_settings 
        SET business_enabled = ?, entertainment_enabled = ?, sports_enabled = ?, 
            technology_enabled = ?, keywords_enabled = ?, keywords = ?, email = ?
        WHERE user_id = ?
    )";
    
    try {
        auto stmt = conn->prepareStatement(query);
        stmt->setBoolean(1, settings.businessEnabled);
        stmt->setBoolean(2, settings.entertainmentEnabled);
        stmt->setBoolean(3, settings.sportsEnabled);
        stmt->setBoolean(4, settings.technologyEnabled);
        stmt->setBoolean(5, settings.keywordsEnabled);
        stmt->setString(6, settings.keywords);
        stmt->setString(7, settings.email);
        stmt->setInt(8, settings.userId);
        
        return stmt->executeUpdate() > 0;
    } catch (const sql::SQLException& e) {
        std::cerr << "Error updating user notification settings: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationRepository::updateCategorySettings(int userId, int categoryId, bool enabled) {
    auto conn = dbConn->getConnection();
    
    std::string categoryField;
    switch (categoryId) {
        case 1: categoryField = "business_enabled"; break;
        case 2: categoryField = "entertainment_enabled"; break;
        case 3: categoryField = "sports_enabled"; break;
        case 4: categoryField = "technology_enabled"; break;
        default: return false;
    }
    
    std::string query = "UPDATE user_notification_settings SET " + categoryField + " = ? WHERE user_id = ?";
    
    try {
        auto stmt = conn->prepareStatement(query);
        stmt->setBoolean(1, enabled);
        stmt->setInt(2, userId);
        
        return stmt->executeUpdate() > 0;
    } catch (const sql::SQLException& e) {
        std::cerr << "Error updating category settings: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationRepository::updateKeywords(int userId, const std::string& keywords) {
    auto conn = dbConn->getConnection();
    
    std::string query = "UPDATE user_notification_settings SET keywords = ? WHERE user_id = ?";
    
    try {
        auto stmt = conn->prepareStatement(query);
        stmt->setString(1, keywords);
        stmt->setInt(2, userId);
        
        return stmt->executeUpdate() > 0;
    } catch (const sql::SQLException& e) {
        std::cerr << "Error updating keywords: " << e.what() << std::endl;
        return false;
    }
}

std::vector<int> NotificationRepository::getUsersToNotifyForCategory(const std::string& categoryName) {
    auto conn = dbConn->getConnection();
    std::vector<int> userIds;
    
    std::string categoryField;
    if (categoryName == "business") categoryField = "business_enabled";
    else if (categoryName == "entertainment") categoryField = "entertainment_enabled";
    else if (categoryName == "sports") categoryField = "sports_enabled";
    else if (categoryName == "technology") categoryField = "technology_enabled";
    else return userIds;
    
    std::string query = "SELECT user_id FROM user_notification_settings WHERE " + categoryField + " = TRUE";
    
    try {
        auto stmt = conn->createStatement();
        auto rs = stmt->executeQuery(query);
        
        while (rs->next()) {
            userIds.push_back(rs->getInt("user_id"));
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "Error getting users for category notification: " << e.what() << std::endl;
    }
    
    return userIds;
}

std::vector<int> NotificationRepository::getUsersToNotifyForKeywords(const std::string& articleTitle, const std::string& articleDescription) {
    auto conn = dbConn->getConnection();
    std::vector<int> userIds;
    
    std::string query = "SELECT user_id, keywords FROM user_notification_settings WHERE keywords_enabled = TRUE AND keywords IS NOT NULL AND keywords != ''";
    
    try {
        auto stmt = conn->createStatement();
        auto rs = stmt->executeQuery(query);
        
        std::string combinedText = articleTitle + " " + articleDescription;
        std::transform(combinedText.begin(), combinedText.end(), combinedText.begin(), ::tolower);
        
        while (rs->next()) {
            int userId = rs->getInt("user_id");
            std::string keywords = rs->getString("keywords");
            
            std::istringstream keywordStream(keywords);
            std::string keyword;
            while (std::getline(keywordStream, keyword, ',')) {
                keyword.erase(0, keyword.find_first_not_of(" \t"));
                keyword.erase(keyword.find_last_not_of(" \t") + 1);
                std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
                
                if (!keyword.empty() && combinedText.find(keyword) != std::string::npos) {
                    userIds.push_back(userId);
                    break; 
                }
            }
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "Error getting users for keyword notification: " << e.what() << std::endl;
    }
    
    return userIds;
} 