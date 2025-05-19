#include "Account.h"

Account::Account(double initialBalance, double limit)
        : balance(initialBalance), dailyLimit(limit), dailyWithdrawn(0.0) {}

void Account::withdraw(double amount) {
    if (amount <= 0) throw ATMException("Invalid withdrawal amount.");
    if (amount > balance) throw ATMException("Insufficient account balance.");
    if ((dailyWithdrawn + amount) > dailyLimit) throw ATMException("Daily limit exceeded.");

    balance -= amount;
    dailyWithdrawn += amount;
}

void Account::resetLimit() { dailyWithdrawn = 0.0; }

double Account::getBalance() const { return balance; }

double Account::getRemainingLimit() const { return dailyLimit - dailyWithdrawn; }
