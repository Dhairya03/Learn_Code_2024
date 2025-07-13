#ifndef EXTERNAL_SERVER_SERVICE_H
#define EXTERNAL_SERVER_SERVICE_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "../../Core/inc/Client.h"

class ExternalServerService {
private:
    Client& client;

public:
    ExternalServerService(Client& client);
    
    std::vector<nlohmann::json> getServerStatus();
    std::vector<nlohmann::json> getServerDetails();
    bool updateServerApiKey(int serverId, const std::string& newApiKey);
};

#endif

