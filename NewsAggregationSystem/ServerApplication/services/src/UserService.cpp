#include "services/inc/UserService.h"
#include "repositories/inc/UserRepository.h"
#include "services/inc/NotificationService.h"
#include "models/inc/User.h"
#include <iostream>

UserService::UserService(std::shared_ptr<DBConnection> dbConn) : dbConn(std::move(dbConn)) {}

bool UserService::signup(const std::string& username, const std::string& email, const std::string& password) {
    std::cout << "[UserService] signup called" << std::endl;
    try {
        User user;
        user.username = username;
        user.email = email;
        user.password = password;
        UserRepository repo(dbConn);
        bool userCreated = repo.createUser(user);
        if (userCreated) {
            NotificationService notificationService(dbConn);
            notificationService.createUserNotificationSettings(user.id, email);
        }
        std::cout << "[UserService] signup success" << std::endl;
        return userCreated;
    } catch (const std::exception& e) {
        std::cerr << "[UserService] signup error: " << e.what() << std::endl;
        return false;
    }
}
