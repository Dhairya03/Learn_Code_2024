#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include "models/inc/User.h"
#include <optional>
#include <string>
#include <memory>
#include "database/inc/DBConnection.h"

class UserRepository {
public:
    explicit UserRepository(std::shared_ptr<DBConnection> dbConn);
    bool createUser(const User& user);
    std::optional<User> findByEmailAndPassword(const std::string& email, const std::string& password);

private:
    std::shared_ptr<DBConnection> db;
};

#endif