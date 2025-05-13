#include "Atm.h"
#include "BankServer.h"
#include <iostream>

int main() {
    BankServer server;
    ATM atm(server);
    atm.run();

    std::cout << "Thank you for using the ATM.\n";
    return 0;
}