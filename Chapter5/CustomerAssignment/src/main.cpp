#include <iostream>
#include "Customer.h"
#include "Wallet.h"

int main()
{
    try
    {
        Customer customer("Dhairya", "Gupta", 50.0f);
        Wallet wallet = customer.getWallet();
        float amount = 2.0f;

        std::cout << "Need to make a payment of Rs." << amount << " from " << customer.getFirstName() << " " << customer.getLastName() << std::endl;

        if (wallet.getTotalMoney() >= amount)
        {
            wallet.subtractMoney(amount);
            std::cout << "Payment received. "
                      << "Remaining balance: Rs." << wallet.getTotalMoney() << std::endl;
        }
        else
        {
            std::cout << "Not enough money. Come back later.\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    return 0;
}
 