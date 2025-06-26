#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <string>
#include <memory>
#include "database/inc/DBConnection.h"

class UserService {
public:
    explicit UserService(std::shared_ptr<DBConnection> dbConn); 
    bool signup(const std::string& username,
                const std::string& email,
                const std::string& password);

private:
    std::shared_ptr<DBConnection> dbConn;
};

#endif