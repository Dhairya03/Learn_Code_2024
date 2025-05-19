#include "gtest/gtest.h"
#include "Account.h"
#include "Authenticator.h"
#include "AtmMachine.h"
#include "AtmException.h"
#include "Atm.h"
#include "BankServer.h"
#include "IServerConnection.h"
#include <iostream>
TEST(AuthenticatorTest, CorrectPIN) {
    Authenticator auth;
    EXPECT_NO_THROW(auth.verifyPin(1234));
    EXPECT_FALSE(auth.isBlocked());
}

TEST(AuthenticatorTest, InvalidPINThenCorrectPIN) {
    Authenticator auth;
    EXPECT_THROW(auth.verifyPin(1111), ATMException);
    EXPECT_NO_THROW(auth.verifyPin(1234));
}

TEST(AuthenticatorTest, CardBlockedAfterThreeAttempts) {
    Authenticator auth;
    EXPECT_THROW(auth.verifyPin(1111), ATMException);
    EXPECT_THROW(auth.verifyPin(2222), ATMException);
    EXPECT_THROW(auth.verifyPin(3333), ATMException);
    EXPECT_TRUE(auth.isBlocked());
}
