#include "../inc/Session.h"

void Session::setUser(const std::string& uname, const std::string& r, int uid) {
    username = uname;
    role = r;
    userId = uid;
}

void Session::clear() {
    username.clear();
    role.clear();
    userId = -1;
}

std::string Session::getUsername() const { return username; }
std::string Session::getRole() const { return role; }
int Session::getUserId() const { return userId; }

bool Session::isLoggedIn() const { return !username.empty() && userId != -1; }
