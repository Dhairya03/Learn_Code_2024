#include "services/inc/AuthService.h"
#include "repositories/inc/UserRepository.h"

AuthService::AuthService(std::shared_ptr<DBConnection> dbConn) : db(std::move(dbConn)) {}

std::optional<User> AuthService::login(const std::string& email, const std::string& password) {
    UserRepository repo(db);
    return repo.findByEmailAndPassword(email, password);
}


bool AuthService::logout() {
    return true;
}