#include "gtest/gtest.h"
#include "Account.h"
#include "Authenticator.h"
#include "AtmMachine.h"
#include "AtmException.h"
#include "Atm.h"
#include "BankServer.h"
#include "IServerConnection.h"
#include <iostream>

class MockServerConnection : public IServerConnection
{
public:
    bool connect() override { return true; } // always succeed
};

class FailingServer : public IServerConnection
{
public:
    bool connect() override { return false; }
};

TEST(ATMTest, ATMServerConnectionFailsGracefully)
{

    FailingServer failingServer;
    ATM atm(failingServer);

    testing::internal::CaptureStdout();
    atm.run();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Unable to connect to the server."), std::string::npos);
}

TEST(ATMTest, ATMServerConnectionSuccess)
{
    MockServerConnection mockServer;
    ATM atm(mockServer);

    testing::internal::CaptureStdout();
    atm.run();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Thank you for using the ATM."), std::string::npos);
}