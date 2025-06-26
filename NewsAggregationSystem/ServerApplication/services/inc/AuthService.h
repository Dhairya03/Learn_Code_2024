#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include "models/inc/User.h"
#include <optional>
#include <string>
#include <memory>
#include "database/inc/DBConnection.h"

class AuthService {
public:
    explicit AuthService(std::shared_ptr<DBConnection> dbConn);
    std::optional<User> login(const std::string& email, const std::string& password);
    bool logout();

private:
    std::shared_ptr<DBConnection> db;
};

#endif