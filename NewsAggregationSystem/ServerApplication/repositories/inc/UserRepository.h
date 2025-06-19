#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include "models/inc/User.h"

class UserRepository {
public:
    bool createUser(const User& user);
};

#endif