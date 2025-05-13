#include "BankServer.h"

bool BankServer::connect()
{
    srand(static_cast<unsigned>(time(nullptr)));
    return rand() % 10 != 0; // Simulate 10% failure chance
}
