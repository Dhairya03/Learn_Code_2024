#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "Wallet.h"

class Customer {
private:
    std::string firstName;
    std::string lastName;
    Wallet wallet;

public:
    Customer(std::string firstName, std::string lastName, float initialBalance = 0.0f);

    std::string getFirstName() const;
    std::string getLastName() const;
    Wallet getWallet() const;
};

#endif
