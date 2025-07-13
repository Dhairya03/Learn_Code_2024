#include "../inc/ExternalServerService.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ExternalServerService::ExternalServerService(Client& client) : client(client) {}

std::vector<nlohmann::json> ExternalServerService::getServerStatus() {
    std::vector<nlohmann::json> servers;
    try {
        std::string response = client.get("/admin/servers/status");
        auto responseJson = json::parse(response);
        
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data")) {
            auto data = responseJson["data"];
            if (data.is_array()) {
                for (const auto& server : data) {
                    servers.push_back(server);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting server status: " << e.what() << std::endl;
    }
    return servers;
}

std::vector<nlohmann::json> ExternalServerService::getServerDetails() {
    std::vector<nlohmann::json> servers;
    try {
        std::string response = client.get("/admin/servers/details");
        auto responseJson = json::parse(response);
        
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data")) {
            auto data = responseJson["data"];
            if (data.is_array()) {
                for (const auto& server : data) {
                    servers.push_back(server);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting server details: " << e.what() << std::endl;
    }
    return servers;
}

bool ExternalServerService::updateServerApiKey(int serverId, const std::string& newApiKey) {
    try {
        json body = {
            {"server_id", serverId},
            {"api_key", newApiKey}
        };
        
        std::string response = client.put("/admin/servers/update", body.dump());
        auto responseJson = json::parse(response);
        
        return responseJson.contains("success") && responseJson["success"].get<bool>();
    } catch (const std::exception& e) {
        std::cerr << "Error updating server API key: " << e.what() << std::endl;
        return false;
    }
}

