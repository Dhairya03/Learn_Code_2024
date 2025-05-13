#include "Atm.h"

ATM::ATM(IServerConnection &serverConn)
    : server(serverConn), account(1000.0, 500.0), machine(10000.0) {}

void ATM::run()
{
    try
    {
        if (!server.connect())
            throw ATMException("Unable to connect to the server.");

        authenticateUser();
        operateMenu();
    }
    catch (const ATMException &ex)
    {
        std::cout << "ATM Error: " << ex.what() << std::endl;
    }
}

void ATM::authenticateUser()
{
    int pin;
    while (true)
    {
        std::cout << "Enter PIN: ";
        std::cin >> pin;
        try
        {
            authenticator.verifyPin(pin);
            break;
        }
        catch (const ATMException &ex)
        {
            std::cout << "Authentication Failed: " << ex.what() << std::endl;
            if (authenticator.isBlocked())
                throw;
        }
    }
}

void ATM::operateMenu()
{
    char choice;
    do
    {
        double amount;
        std::cout << "\nEnter amount to withdraw: ";
        std::cin >> amount;
        try
        {
            account.withdraw(amount);
            machine.dispense(amount);
            std::cout << "Dispensed $" << amount << ". Remaining balance: $" << account.getBalance() << std::endl;
        }
        catch (const ATMException &ex)
        {
            std::cout << "Transaction Error: " << ex.what() << std::endl;
        }

        std::cout << "Perform another transaction? (y/n): ";
        std::cin >> choice;

    } while (choice == 'y' || choice == 'Y');
}
