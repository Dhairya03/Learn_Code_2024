#include "services/inc/UserService.h"
#include "repositories/inc/UserRepository.h"
#include "services/inc/NotificationService.h"
#include "models/inc/User.h"

UserService::UserService(std::shared_ptr<DBConnection> dbConn) : dbConn(std::move(dbConn)) {}

bool UserService::signup(const std::string& username, const std::string& email, const std::string& password) {
    User user;
    user.username = username;
    user.email = email;
    user.password = password;

    UserRepository repo(dbConn);
    bool userCreated = repo.createUser(user);
    
    if (userCreated) {
        // Create notification settings for the new user
        NotificationService notificationService(dbConn);
        notificationService.createUserNotificationSettings(user.id, email);
    }
    
    return userCreated;
}
