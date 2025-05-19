#include <gtest/gtest.h>
#include "Account.h"
#include "Authenticator.h"
#include "AtmMachine.h"
#include "AtmException.h"

TEST(AccountTest, WithdrawWithinLimit) {
    Account account(1000, 500);
    EXPECT_NO_THROW(account.withdraw(200));
    EXPECT_DOUBLE_EQ(account.getBalance(), 800);
}

TEST(AccountTest, InsufficientFundsThrowsATMException) {
    Account account(100, 500);
    EXPECT_THROW(account.withdraw(200), ATMException);
}

TEST(AccountTest, DailyLimitExceededThrowsATMException) {
    Account account(1000, 300);
    EXPECT_NO_THROW(account.withdraw(200));
    EXPECT_THROW(account.withdraw(200), ATMException);
}

TEST(AccountTest, InvalidAmountThrowsATMException) {
    Account account(1000, 500);
    EXPECT_THROW(account.withdraw(0), ATMException);
    EXPECT_THROW(account.withdraw(-50), ATMException);
}
