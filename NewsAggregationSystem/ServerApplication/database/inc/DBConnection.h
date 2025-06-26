#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <cppconn/connection.h>
#include <memory>
#include <string>

class DBConnection {
public:
    bool connect(const std::string& host,
                 const std::string& user,
                 const std::string& password,
                 const std::string& database);
    std::shared_ptr<sql::Connection> getConnection();
    bool isConnected() const;

private:
    std::shared_ptr<sql::Connection> conn;
};

#endif