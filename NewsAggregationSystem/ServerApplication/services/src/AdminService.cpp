#include "services/inc/AdminService.h"

AdminService::AdminService(std::shared_ptr<DBConnection> db) : db(std::move(db)) {}

std::vector<ExternalServer> AdminService::getExternalServers() {
    ExternalServerRepository repo(db);
    return repo.getAllServers();
}

std::vector<ExternalServer> AdminService::getExternalServerDetails() {
    ExternalServerRepository repo(db);
    return repo.getAllServerDetails();
}

bool AdminService::updateServerApiKey(int id, const std::string& key) {
    ExternalServerRepository repo(db);
    return repo.updateApiKey(id, key);
}

bool AdminService::addCategory(const std::string& name) {
    CategoryRepository repo(db);
    return repo.addCategory(name);
}
