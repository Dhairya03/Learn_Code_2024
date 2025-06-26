#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H

#include "repositories/inc/ExternalServerRepository.h"
#include "repositories/inc/CategoryRepository.h"
#include "database/inc/DBConnection.h"
#include <memory>

class AdminService {
public:
    explicit AdminService(std::shared_ptr<DBConnection> db);
    std::vector<ExternalServer> getExternalServers();
    std::vector<ExternalServer> getExternalServerDetails();
    bool updateServerApiKey(int id, const std::string& key);
    bool addCategory(const std::string& name);
private:
    std::shared_ptr<DBConnection> db;
};

#endif