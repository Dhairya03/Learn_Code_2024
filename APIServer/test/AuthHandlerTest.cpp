#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "AuthHandler.h"
#include <crow.h>
#include <mysql/mysql.h>
#include "MockMySql.h"

class AuthHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockMYSQL = new MockMYSQL();
        authHandler = new AuthHandler(reinterpret_cast<MYSQL*>(mockMYSQL));
    }

    void TearDown() override {
        delete authHandler;
        delete mockMYSQL;
    }

    MockMYSQL* mockMYSQL;
    AuthHandler* authHandler;
};

TEST_F(AuthHandlerTest, HandleSignupSuccess) {
    crow::request req;
    req.body = R"({"username": "test_user", "password": "test_password"})";

    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(0));

    std::string result = authHandler->handleSignup(req);
    EXPECT_EQ(result, "Signup successful");
}

TEST_F(AuthHandlerTest, HandleSignupFailure) {
    crow::request req;
    req.body = R"({"username": "test_user", "password": "test_password"})";

    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(1));

    std::string result = authHandler->handleSignup(req);
    EXPECT_EQ(result, "Signup failed");
}

TEST_F(AuthHandlerTest, HandleSignupInvalidJSON) {
    crow::request req;
    req.body = "Invalid JSON";

    std::string result = authHandler->handleSignup(req);
    EXPECT_EQ(result, "Invalid JSON");
}

TEST_F(AuthHandlerTest, HandleLoginSuccess) {
    crow::request req;
    req.body = R"({"username": "test_user", "password": "test_password"})";

    MYSQL_RES mockRes;
    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockMYSQL, mysql_store_result(::testing::_))
        .WillOnce(::testing::Return(&mockRes));
    EXPECT_CALL(*mockMYSQL, mysql_num_rows(&mockRes))
        .WillOnce(::testing::Return(1));
    EXPECT_CALL(*mockMYSQL, mysql_free_result(&mockRes));

    std::string result = authHandler->handleLogin(req);
    EXPECT_EQ(result, "Login successful");
}

TEST_F(AuthHandlerTest, HandleLoginInvalidCredentials) {
    crow::request req;
    req.body = R"({"username": "test_user", "password": "wrong_password"})";

    MYSQL_RES mockRes;
    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockMYSQL, mysql_store_result(::testing::_))
        .WillOnce(::testing::Return(&mockRes));
    EXPECT_CALL(*mockMYSQL, mysql_num_rows(&mockRes))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockMYSQL, mysql_free_result(&mockRes));

    std::string result = authHandler->handleLogin(req);
    EXPECT_EQ(result, "Invalid credentials");
}

TEST_F(AuthHandlerTest, HandleLoginInvalidJSON) {
    crow::request req;
    req.body = "Invalid JSON";

    std::string result = authHandler->handleLogin(req);
    EXPECT_EQ(result, "Invalid JSON");
}

TEST_F(AuthHandlerTest, HandleLoginQueryFailure) {
    crow::request req;
    req.body = R"({"username": "test_user", "password": "test_password"})";

    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(1));

    std::string result = authHandler->handleLogin(req);
    EXPECT_EQ(result, "Login failed");
}