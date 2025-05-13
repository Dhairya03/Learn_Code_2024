#include "AtmMachine.h"

void ATMMachine::dispense(double amount)
{
    if (amount > availableCash)
        throw ATMException("ATM does not have sufficient cash.");
    availableCash -= amount;
}

double ATMMachine::getAvailableCash() const { return availableCash; }
