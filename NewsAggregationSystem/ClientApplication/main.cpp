#include "Client.h"
#include <iostream>

int main() {
    std::cout << "News Aggregation System" << std::endl;
    std::cout << "1. Signup\n";
    std::cout << "Choose an option: ";
    
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        signupUser();
    }

    return 0;
}
