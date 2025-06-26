#ifndef AUTH_CONTROLLER_H
#define AUTH_CONTROLLER_H

#include <lib/crow/crow_all.h>
#include <memory>
#include "database/inc/DBConnection.h"  

class AuthController {
public:
    static crow::response login(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response logout(const crow::request& req, std::shared_ptr<DBConnection> dbConn);  
};

#endif