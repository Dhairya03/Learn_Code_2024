#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include <lib/crow/crow_all.h>

class UserController {
public:
    static crow::response signup(const crow::request& req);
};

#endif