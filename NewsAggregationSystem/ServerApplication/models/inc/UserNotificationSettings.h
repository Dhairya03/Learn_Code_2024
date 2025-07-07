#ifndef USER_NOTIFICATION_SETTINGS_H
#define USER_NOTIFICATION_SETTINGS_H

#include <string>

class UserNotificationSettings {
public:
    int id = 0;
    int userId;
    bool businessEnabled = false;
    bool entertainmentEnabled = false;
    bool sportsEnabled = false;
    bool technologyEnabled = false;
    bool keywordsEnabled = false;
    std::string keywords; // Comma-separated keywords
    std::string email; // User's email for email notifications
};

#endif 