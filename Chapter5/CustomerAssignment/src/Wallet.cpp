#include "Wallet.h"

Wallet::Wallet(float initialValue) : walletBalance(initialValue) {
    if(initialValue < 0) {
        throw std::invalid_argument("Initial balance cannot be negative.");
    }
}

float Wallet::getTotalMoney() const {
    return walletBalance;
}

void Wallet::addMoney(float depositedAmount) {
    walletBalance += depositedAmount;
}

void Wallet::subtractMoney(float debitedAmount) {
    if(debitedAmount < 0) {
        throw std::invalid_argument("Debit amount cannot be negative.");
    }
    if(debitedAmount > walletBalance) {
        throw std::invalid_argument("Insufficient funds for this transaction.");
    }
    walletBalance -= debitedAmount;
}

