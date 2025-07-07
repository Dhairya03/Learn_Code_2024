#include "services/inc/AdminService.h"
#include <iostream>

AdminService::AdminService(std::shared_ptr<DBConnection> db) : db(std::move(db)) {}

std::vector<ExternalServer> AdminService::getExternalServers() {
    std::cout << "[AdminService] getExternalServers called" << std::endl;
    try {
        ExternalServerRepository repo(db);
        auto result = repo.getAllServers();
        std::cout << "[AdminService] getExternalServers success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[AdminService] getExternalServers error: " << e.what() << std::endl;
        return {};
    }
}

std::vector<ExternalServer> AdminService::getExternalServerDetails() {
    std::cout << "[AdminService] getExternalServerDetails called" << std::endl;
    try {
        ExternalServerRepository repo(db);
        auto result = repo.getAllServerDetails();
        std::cout << "[AdminService] getExternalServerDetails success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[AdminService] getExternalServerDetails error: " << e.what() << std::endl;
        return {};
    }
}

bool AdminService::updateServerApiKey(int id, const std::string& key) {
    std::cout << "[AdminService] updateServerApiKey called" << std::endl;
    try {
        ExternalServerRepository repo(db);
        bool result = repo.updateApiKey(id, key);
        std::cout << "[AdminService] updateServerApiKey success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[AdminService] updateServerApiKey error: " << e.what() << std::endl;
        return false;
    }
}

bool AdminService::addCategory(const std::string& name) {
    std::cout << "[AdminService] addCategory called" << std::endl;
    try {
        CategoryRepository repo(db);
        bool result = repo.addCategory(name);
        std::cout << "[AdminService] addCategory success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[AdminService] addCategory error: " << e.what() << std::endl;
        return false;
    }
}
