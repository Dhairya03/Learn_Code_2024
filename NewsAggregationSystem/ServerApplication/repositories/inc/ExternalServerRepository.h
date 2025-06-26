#ifndef EXTERNAL_SERVER_REPOSITORY_H
#define EXTERNAL_SERVER_REPOSITORY_H

#include "database/inc/DBConnection.h"
#include <vector>
#include <string>
#include <memory>
#include "models/inc/ExternalServer.h"

class ExternalServerRepository {
public:
    explicit ExternalServerRepository(std::shared_ptr<DBConnection> db);
    std::vector<ExternalServer> getAllServers();
    std::vector<ExternalServer> getAllServerDetails();
    bool updateApiKey(int id, const std::string& newKey);

private:
    std::shared_ptr<DBConnection> db;
};

#endif
