#ifndef WALLET_H
#define WALLET_H

#include <iostream>
#include <stdexcept>

class Wallet {
private:
    float walletBalance;

public:
    Wallet(float initialValue = 0.0f);
    float getTotalMoney() const;
    void addMoney(float deposit);
    void subtractMoney(float debit);
};

#endif
