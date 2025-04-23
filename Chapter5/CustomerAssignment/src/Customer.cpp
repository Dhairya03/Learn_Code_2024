#include "Customer.h"

Customer::Customer(std::string firstName, std::string lastName, float initialBalance)
    : firstName(std::move(firstName)), lastName(std::move(lastName)), wallet(initialBalance) {}

std::string Customer::getFirstName() const {
    return firstName;
}

std::string Customer::getLastName() const {
    return lastName;
}   

Wallet Customer::getWallet() const {
    return wallet;
}

