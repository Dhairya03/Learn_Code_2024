#include "../inc/Validator.h"
#include <regex>

bool Validator::isValidEmail(const std::string& email) {
    const std::regex pattern(R"(^[\w\.-]+@[\w\.-]+\.\w{2,}$)");
    return std::regex_match(email, pattern);
}

bool Validator::isValidUsername(const std::string& username) {
    const std::regex pattern(R"(^[a-zA-Z0-9_]{3,20}$)");
    return std::regex_match(username, pattern);
}

bool Validator::isValidPassword(const std::string& password) {
    const std::regex pattern(R"(^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d!@#$%^&*]{6,}$)");
    return std::regex_match(password, pattern);
}
