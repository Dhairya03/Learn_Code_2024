#include "controllers/inc/AdminController.h"
#include "services/inc/AdminService.h"
#include <lib/json/json.hpp>

using json = nlohmann::json;

crow::response AdminController::getExternalServers(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    AdminService service(dbConn);
    auto servers = service.getExternalServers();

    json result = json::array();
    for (const auto& s : servers) {
        result.push_back({
            {"id", s.id},
            {"name", s.name},
            {"status", s.isActive ? "Active" : "Not Active"},
            {"last_accessed", s.lastAccessed}
        });
    }
    return crow::response(200, result.dump());
}

crow::response AdminController::getExternalServerDetails(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    AdminService service(dbConn);
    auto servers = service.getExternalServerDetails();

    json result = json::array();
    for (const auto& s : servers) {
        result.push_back({
            {"id", s.id},
            {"name", s.name},
            {"api_key", s.apiKey}
        });
    }
    return crow::response(200, result.dump());
}

crow::response AdminController::updateApiKey(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    auto body = json::parse(req.body);
    if (!body.contains("id") || !body.contains("api_key"))
        return crow::response(400, "Missing id or api_key");

    int id = body["id"];
    std::string key = body["api_key"];

    AdminService service(dbConn);
    if (service.updateServerApiKey(id, key)) {
        return crow::response(200, "API key updated successfully");
    }
    return crow::response(500, "Failed to update API key");
}

crow::response AdminController::addCategory(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    auto body = json::parse(req.body);
    if (!body.contains("name"))
        return crow::response(400, "Missing category name");

    AdminService service(dbConn);
    if (service.addCategory(body["name"])) {
        return crow::response(200, "Category added");
    }
    return crow::response(500, "Failed to add category");
}
