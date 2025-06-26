#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include <lib/crow/crow_all.h>
#include <memory>
#include "database/inc/DBConnection.h"

class UserController {
public:
    static crow::response signup(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
};

#endif