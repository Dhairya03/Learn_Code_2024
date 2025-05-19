#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ShoppingServer.h"
#include "AuthHandler.h"
#include "ProductHandler.h"
#include "CartHandler.h"
#include "OrderHandler.h"
#include <crow.h>
#include <mysql/mysql.h>
#include "MockMySql.h"

class MockAuthHandler {
public:
    MOCK_METHOD(std::string, handleSignup, (const crow::request& req), ());
    MOCK_METHOD(std::string, handleLogin, (const crow::request& req), ());
};

class MockProductHandler {
public:
    MOCK_METHOD(std::string, handleGetCategories, (), ());
    MOCK_METHOD(std::string, handleGetProducts, (), ());
};

class MockCartHandler {
public:
    MOCK_METHOD(std::string, handleAddToCart, (const crow::request& req), ());
    MOCK_METHOD(std::string, handleViewCart, (), ());
    MOCK_METHOD(std::string, handleBuyNow, (), ());
};

class MockOrderHandler {
public:
    MOCK_METHOD(std::string, handleOrderHistory, (), ());
};

class ShoppingServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockAuthHandler = new MockAuthHandler();
        mockProductHandler = new MockProductHandler();
        mockCartHandler = new MockCartHandler();
        mockOrderHandler = new MockOrderHandler();

        shoppingServer = new ShoppingServer();
        shoppingServer->authHandler = reinterpret_cast<AuthHandler*>(mockAuthHandler);
        shoppingServer->productHandler = reinterpret_cast<ProductHandler*>(mockProductHandler);
        shoppingServer->cartHandler = reinterpret_cast<CartHandler*>(mockCartHandler);
        shoppingServer->orderHandler = reinterpret_cast<OrderHandler*>(mockOrderHandler);
    }

    void TearDown() override {
        delete shoppingServer;
    }

    MockAuthHandler* mockAuthHandler;
    MockProductHandler* mockProductHandler;
    MockCartHandler* mockCartHandler;
    MockOrderHandler* mockOrderHandler;
    ShoppingServer* shoppingServer;
};

TEST_F(ShoppingServerTest, SignupRoute) {
    crow::request req;
    req.body = R"({"username": "test_user", "password": "test_password"})";

    EXPECT_CALL(*mockAuthHandler, handleSignup(::testing::_))
        .WillOnce(::testing::Return("Signup successful"));

    auto response = shoppingServer->authHandler->handleSignup(req);
    EXPECT_EQ(response, "Signup successful");
}

TEST_F(ShoppingServerTest, LoginRoute) {
    crow::request req;
    req.body = R"({"username": "test_user", "password": "test_password"})";

    EXPECT_CALL(*mockAuthHandler, handleLogin(::testing::_))
        .WillOnce(::testing::Return("Login successful"));

    auto response = shoppingServer->authHandler->handleLogin(req);
    EXPECT_EQ(response, "Login successful");
}

TEST_F(ShoppingServerTest, GetCategoriesRoute) {
    EXPECT_CALL(*mockProductHandler, handleGetCategories())
        .WillOnce(::testing::Return(R"(["Electronics", "Clothing"])"));

    auto response = shoppingServer->productHandler->handleGetCategories();
    EXPECT_EQ(response, R"(["Electronics", "Clothing"])");
}

TEST_F(ShoppingServerTest, GetProductsRoute) {
    EXPECT_CALL(*mockProductHandler, handleGetProducts())
        .WillOnce(::testing::Return(R"([{"id":1, "name":"Laptop"}, {"id":2, "name":"T-Shirt"}])"));

    auto response = shoppingServer->productHandler->handleGetProducts();
    EXPECT_EQ(response, R"([{"id":1, "name":"Laptop"}, {"id":2, "name":"T-Shirt"}])");
}

TEST_F(ShoppingServerTest, AddToCartRoute) {
    crow::request req;
    req.body = R"({"user_id": 1, "product_id": 101, "quantity": 2})";

    EXPECT_CALL(*mockCartHandler, handleAddToCart(::testing::_))
        .WillOnce(::testing::Return("Product added to cart"));

    auto response = shoppingServer->cartHandler->handleAddToCart(req);
    EXPECT_EQ(response, "Product added to cart");
}

TEST_F(ShoppingServerTest, ViewCartRoute) {
    EXPECT_CALL(*mockCartHandler, handleViewCart())
        .WillOnce(::testing::Return(R"([{"product_id":101, "quantity":2}, {"product_id":102, "quantity":1}])"));

    auto response = shoppingServer->cartHandler->handleViewCart();
    EXPECT_EQ(response, R"([{"product_id":101, "quantity":2}, {"product_id":102, "quantity":1}])");
}

TEST_F(ShoppingServerTest, BuyNowRoute) {
    EXPECT_CALL(*mockCartHandler, handleBuyNow())
        .WillOnce(::testing::Return("Order placed successfully"));

    auto response = shoppingServer->cartHandler->handleBuyNow();
    EXPECT_EQ(response, "Order placed successfully");
}

TEST_F(ShoppingServerTest, OrderHistoryRoute) {
    EXPECT_CALL(*mockOrderHandler, handleOrderHistory())
        .WillOnce(::testing::Return(R"([{"order_id":1, "product_id":101, "quantity":2}])"));

    auto response = shoppingServer->orderHandler->handleOrderHistory();
    EXPECT_EQ(response, R"([{"order_id":1, "product_id":101, "quantity":2}])");
}