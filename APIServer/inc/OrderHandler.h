#ifndef ORDER_HANDLER_H
#define ORDER_HANDLER_H

#include <mysql/mysql.h>
#include <crow_all.h>

class OrderHandler {
private:
    MYSQL *conn;
public:
    OrderHandler(MYSQL *connection);
    std::string handleOrderHistory();
};

#endif
