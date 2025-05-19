#ifndef ATMMACHINE_H
#define ATMMACHINE_H

#include "AtmException.h"
#include <iostream>

class ATMMachine {
    private:
        double availableCash;
    
    public:
        explicit ATMMachine(double initialCash) : availableCash(initialCash) {}
    
        void dispense(double amount);
    
        double getAvailableCash() const ;
    };

    #endif 