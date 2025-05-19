#include "gtest/gtest.h"
#include "Account.h"
#include "Authenticator.h"
#include "AtmMachine.h"
#include "AtmException.h"
#include "Atm.h"
#include "BankServer.h"
#include "IServerConnection.h"
#include <iostream>

TEST(ATMMachineTest, DispenseSuccess) {
    ATMMachine atm(5000);
    EXPECT_NO_THROW(atm.dispense(1000));
    EXPECT_DOUBLE_EQ(atm.getAvailableCash(), 4000);
}

TEST(ATMMachineTest, DispenseFailureThrowsATMException) {
    ATMMachine atm(500);
    EXPECT_THROW(atm.dispense(1000), ATMException);
}
