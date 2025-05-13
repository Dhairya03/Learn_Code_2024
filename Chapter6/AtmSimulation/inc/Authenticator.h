#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H
#include "AtmException.h"

class Authenticator
{
private:
    const int correctPin = 1234;
    const int maxAttempts = 3;
    int attempts = 0;
    bool blocked = false;

public:
    void verifyPin(int inputPin);

    bool isBlocked() const;
};

#endif