#ifndef CATEGORY_CONTROLLER_H
#define CATEGORY_CONTROLLER_H

#include <lib/crow/crow_all.h>
#include <memory>
#include "database/inc/DBConnection.h"

class CategoryController {
public:
    static crow::response getAllCategories(const crow::request& req, std::shared_ptr<DBConnection> dbConn);
    static crow::response getCategoryById(const crow::request& req, std::shared_ptr<DBConnection> dbConn, int categoryId);
    static crow::response getCategoryByName(const crow::request& req, std::shared_ptr<DBConnection> dbConn, const std::string& categoryName);
};

#endif 