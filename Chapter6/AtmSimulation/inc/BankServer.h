#ifndef BANKSERVER_H
#define BANKSERVER_H

#include "IServerConnection.h"
#include <cstdlib>
#include <ctime>

class BankServer : public IServerConnection {
public:
    bool connect() override ;
};

#endif