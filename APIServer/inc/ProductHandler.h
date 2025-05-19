#ifndef PRODUCT_HANDLER_H
#define PRODUCT_HANDLER_H

#include <mysql/mysql.h>
#include <crow_all.h>

class ProductHandler {
private:
    MYSQL *conn;
public:
    ProductHandler(MYSQL *connection);
    std::string handleGetCategories();
    std::string handleGetProducts();
};

#endif
