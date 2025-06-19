#include "services/inc/UserService.h"
#include "repositories/inc/UserRepository.h"
#include "models/inc/User.h"

bool UserService::signup(const std::string& username, const std::string& email, const std::string& password) {
    User user;
    user.username = username;
    user.email = email;
    user.password = password;

    UserRepository repo;
    return repo.createUser(user);
}
