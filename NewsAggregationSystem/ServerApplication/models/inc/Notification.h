#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

class Notification {
public:
    int id = 0;
    int userId;
    std::string message;
    std::string type; // "category", "keyword", "system"
    std::string timestamp;
    bool isRead = false;
    int articleId = 0; // Optional: link to specific article
    std::string categoryName; // Optional: for category-based notifications
    std::string keywords; // Optional: for keyword-based notifications
};

#endif 