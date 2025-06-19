#include "DBConnection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <iostream>

bool DBConnection::connect(const std::string& host,
                           const std::string& user,
                           const std::string& password,
                           const std::string& database) {
    try {
        sql::Driver* driver = get_driver_instance();
        conn = std::shared_ptr<sql::Connection>(
            driver->connect(host, user, password));
        conn->setSchema(database);
        return true;
    } catch (sql::SQLException& e) {
        std::cerr << "Connection failed: " << e.what() << std::endl;
        return false;
    }
}

std::shared_ptr<sql::Connection> DBConnection::getConnection() {
    return conn;
}
