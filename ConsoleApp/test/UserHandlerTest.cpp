#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "UserHandler.h"
#include "HttpClient.h"
#include "MockHttpClient.h"

class UserHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockHttpClient = new MockHttpClient();
        userHandler = new UserHandler();
        HttpClient::setMockInstance(mockHttpClient); 
    }

    void TearDown() override {
        delete userHandler;
        delete mockHttpClient;
    }

    MockHttpClient* mockHttpClient;
    UserHandler* userHandler;
};

TEST_F(UserHandlerTest, LoginSuccess) {
    std::istringstream input("test_user\ntest_password\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/login", "POST", R"({"username": "test_user", "password": "test_password"})"))
        .WillOnce(::testing::Return("Login successful"));

    testing::internal::CaptureStdout();
    bool result = userHandler->login();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(result);
    EXPECT_EQ(output, "Enter username: Enter password: Login successful!\n");
}

TEST_F(UserHandlerTest, LoginFailure) {
    std::istringstream input("test_user\nwrong_password\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/login", "POST", R"({"username": "test_user", "password": "wrong_password"})"))
        .WillOnce(::testing::Return("Login failed"));

    testing::internal::CaptureStdout();
    bool result = userHandler->login();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(result);
    EXPECT_EQ(output, "Enter username: Enter password: Login failed.\n");
}

TEST_F(UserHandlerTest, SignupSuccess) {
    std::istringstream input("new_user\nnew_password\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/signup", "POST", R"({"username": "new_user", "password": "new_password"})"))
        .WillOnce(::testing::Return("Signup successful"));

    testing::internal::CaptureStdout();
    userHandler->signup();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Enter new username: Enter new password: Signup successful\n");
}

TEST_F(UserHandlerTest, SignupFailure) {
    std::istringstream input("new_user\nnew_password\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/signup", "POST", R"({"username": "new_user", "password": "new_password"})"))
        .WillOnce(::testing::Return("Signup failed"));

    testing::internal::CaptureStdout();
    userHandler->signup();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Enter new username: Enter new password: Signup failed\n");
}