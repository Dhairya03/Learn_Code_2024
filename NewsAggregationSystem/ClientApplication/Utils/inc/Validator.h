#pragma once
#include <string>

class Validator {
public:
    static bool isValidEmail(const std::string& email);
    static bool isValidUsername(const std::string& username);
    static bool isValidPassword(const std::string& password);
};
