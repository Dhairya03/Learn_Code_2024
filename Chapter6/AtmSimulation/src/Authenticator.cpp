#include "Authenticator.h"

void Authenticator::verifyPin(int inputPin)
{
    if (blocked)
        throw ATMException("Card is blocked.");
    if (inputPin == correctPin)
    {
        attempts = 0;
    }
    else
    {
        attempts++;
        if (attempts >= maxAttempts)
        {
            blocked = true;
            throw ATMException("Card blocked after 3 invalid attempts.");
        }
        else
        {
            throw ATMException("Invalid PIN.");
        }
    }
}

bool Authenticator::isBlocked() const { return blocked; }
