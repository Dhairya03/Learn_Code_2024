#ifndef AUTH_HANDLER_H
#define AUTH_HANDLER_H

#include <mysql/mysql.h>
#include <crow_all.h>

class AuthHandler {
private:
    MYSQL *conn;
public:
    AuthHandler(MYSQL *connection);
    std::string handleSignup(const crow::request& req);
    std::string handleLogin(const crow::request& req);
};

#endif
