#ifndef CATEGORY_REPOSITORY_H
#define CATEGORY_REPOSITORY_H

#include "database/inc/DBConnection.h"
#include <string>
#include <memory>

class CategoryRepository {
public:
    explicit CategoryRepository(std::shared_ptr<DBConnection> db);
    bool addCategory(const std::string& name);
private:
    std::shared_ptr<DBConnection> db;
};

#endif