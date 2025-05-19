#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "CartHandler.h"
#include <crow.h>
#include <mysql/mysql.h>
#include "MockMySql.h"

class CartHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockMYSQL = new MockMYSQL();
        cartHandler = new CartHandler(reinterpret_cast<MYSQL*>(mockMYSQL));
    }

    void TearDown() override {
        delete cartHandler;
        delete mockMYSQL;
    }

    MockMYSQL* mockMYSQL;
    CartHandler* cartHandler;
};

TEST_F(CartHandlerTest, HandleAddToCartSuccess) {
    crow::request req;
    req.body = R"({"user_id": 1, "product_id": 101, "quantity": 2})";

    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(0));

    std::string result = cartHandler->handleAddToCart(req);
    EXPECT_EQ(result, "Product added to cart");
}

TEST_F(CartHandlerTest, HandleAddToCartFailure) {
    crow::request req;
    req.body = R"({"user_id": 1, "product_id": 101, "quantity": 2})";

    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(1));

    std::string result = cartHandler->handleAddToCart(req);
    EXPECT_EQ(result, "Failed to add product to cart");
}

TEST_F(CartHandlerTest, HandleAddToCartInvalidJSON) {
    crow::request req;
    req.body = "Invalid JSON";

    std::string result = cartHandler->handleAddToCart(req);
    EXPECT_EQ(result, "Invalid JSON");
}

TEST_F(CartHandlerTest, HandleViewCartSuccess) {
    MYSQL_RES mockRes;
    MYSQL_ROW mockRow1 = new char*[2]{(char*)"101", (char*)"2"};
    MYSQL_ROW mockRow2 = new char*[2]{(char*)"102", (char*)"1"};

    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockMYSQL, mysql_store_result(::testing::_))
        .WillOnce(::testing::Return(&mockRes));
    EXPECT_CALL(*mockMYSQL, mysql_fetch_row(&mockRes))
        .WillOnce(::testing::Return(mockRow1))
        .WillOnce(::testing::Return(mockRow2))
        .WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(*mockMYSQL, mysql_free_result(&mockRes));

    std::string result = cartHandler->handleViewCart();
    EXPECT_EQ(result, R"([{"product_id":101, "quantity": 2}, {"product_id":102, "quantity": 1}])");

    delete[] mockRow1;
    delete[] mockRow2;
}

TEST_F(CartHandlerTest, HandleBuyNowSuccess) {
    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(0)) 
        .WillOnce(::testing::Return(0)); 

    std::string result = cartHandler->handleBuyNow();
    EXPECT_EQ(result, "Order placed successfully");
}

TEST_F(CartHandlerTest, HandleBuyNowFailure) {
    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(1)); 

    std::string result = cartHandler->handleBuyNow();
    EXPECT_EQ(result, "Failed to place order");
}