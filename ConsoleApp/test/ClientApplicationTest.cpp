#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ClientApplication.h"

class MockUserHandler {
public:
    MOCK_METHOD(bool, login, (), ());
    MOCK_METHOD(void, signup, (), ());
};

class MockShoppingClient {
public:
    MOCK_METHOD(void, browseCategories, (), ());
};

class MockCartHandler {
public:
    MOCK_METHOD(void, viewCart, (), ());
};

class MockOrderHandler {
public:
    MOCK_METHOD(void, handleOrderHistory, (), ());
};

class ClientApplicationTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockUserHandler = new MockUserHandler();
        mockShoppingClient = new MockShoppingClient();
        mockCartHandler = new MockCartHandler();
        mockOrderHandler = new MockOrderHandler();

        clientApp = new ClientApplication();
        clientApp->userHandler = *mockUserHandler;
        clientApp->shoppingClient = *mockShoppingClient;
        clientApp->cartHandler = *mockCartHandler;
        clientApp->orderHandler = *mockOrderHandler;
    }

    void TearDown() override {
        delete clientApp;
        delete mockUserHandler;
        delete mockShoppingClient;
        delete mockCartHandler;
        delete mockOrderHandler;
    }

    MockUserHandler* mockUserHandler;
    MockShoppingClient* mockShoppingClient;
    MockCartHandler* mockCartHandler;
    MockOrderHandler* mockOrderHandler;
    ClientApplication* clientApp;
};

TEST_F(ClientApplicationTest, HandleLoginSuccess) {
    EXPECT_CALL(*mockUserHandler, login())
        .WillOnce(::testing::Return(true));

    testing::internal::CaptureStdout();
    clientApp->handleLogin();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(clientApp->isLoggedIn);
}

TEST_F(ClientApplicationTest, HandleLoginFailure) {
    EXPECT_CALL(*mockUserHandler, login())
        .WillOnce(::testing::Return(false));

    testing::internal::CaptureStdout();
    clientApp->handleLogin();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(clientApp->isLoggedIn);
}

TEST_F(ClientApplicationTest, HandleSignup) {
    EXPECT_CALL(*mockUserHandler, signup())
        .Times(1);

    testing::internal::CaptureStdout();
    clientApp->handleSignup();
    std::string output = testing::internal::GetCapturedStdout();
}

TEST_F(ClientApplicationTest, ShowMainMenuBrowseCategories) {
    EXPECT_CALL(*mockShoppingClient, browseCategories())
        .Times(1);

    std::istringstream input("1\n4\n");
    std::cin.rdbuf(input.rdbuf());

    testing::internal::CaptureStdout();
    clientApp->showMainMenu();
    std::string output = testing::internal::GetCapturedStdout();
}

TEST_F(ClientApplicationTest, ShowMainMenuViewCart) {
    EXPECT_CALL(*mockCartHandler, viewCart())
        .Times(1);

    std::istringstream input("2\n4\n");
    std::cin.rdbuf(input.rdbuf());

    testing::internal::CaptureStdout();
    clientApp->showMainMenu();
    std::string output = testing::internal::GetCapturedStdout();
}

TEST_F(ClientApplicationTest, ShowMainMenuViewOrderHistory) {
    EXPECT_CALL(*mockOrderHandler, handleOrderHistory())
        .Times(1);

    std::istringstream input("3\n4\n");
    std::cin.rdbuf(input.rdbuf());

    testing::internal::CaptureStdout();
    clientApp->showMainMenu();
    std::string output = testing::internal::GetCapturedStdout();
}

TEST_F(ClientApplicationTest, ShowMainMenuLogout) {
    std::istringstream input("4\n");
    std::cin.rdbuf(input.rdbuf());

    testing::internal::CaptureStdout();
    clientApp->showMainMenu();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(clientApp->isLoggedIn);
}