#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "AtmException.h"

class Account {
private:
    double balance;
    double dailyLimit;
    double dailyWithdrawn;

public:
    Account(double initialBalance, double limit);
    void withdraw(double amount);
    void resetLimit();
    double getBalance() const;
    double getRemainingLimit() const;
};

#endif 