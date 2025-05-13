#ifndef ATM_H
#define ATM_H

#include "Authenticator.h"
#include "Account.h"
#include "AtmMachine.h"
#include "IServerConnection.h"
#include "AtmException.h"
#include <iostream>

class ATM {
private:
    Authenticator authenticator;
    Account account;
    ATMMachine machine;
    IServerConnection& server;

public:
    ATM(IServerConnection& serverConn);
    void run();
private:
    void authenticateUser();
    void operateMenu();
};

#endif