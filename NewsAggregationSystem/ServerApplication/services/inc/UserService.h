#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <string>

class UserService {
public:
    bool signup(const std::string& username,
                const std::string& email,
                const std::string& password);
};

#endif