#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "CartHandler.h"
#include "HttpClient.h"
#include "MockHttpClient.h"

class CartHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockHttpClient = new MockHttpClient();
        cartHandler = new CartHandler();
        HttpClient::setMockInstance(mockHttpClient); 
    }

    void TearDown() override {
        delete cartHandler;
        delete mockHttpClient;
    }

    MockHttpClient* mockHttpClient;
    CartHandler* cartHandler;
};

TEST_F(CartHandlerTest, ViewCartSuccess) {
    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/cart", "GET", ""))
        .WillOnce(::testing::Return(R"([{"product_id":101, "quantity":2}, {"product_id":102, "quantity":1}])"));

    testing::internal::CaptureStdout();
    cartHandler->viewCart();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Cart: [{\"product_id\":101, \"quantity\":2}, {\"product_id\":102, \"quantity\":1}]\n");
}

TEST_F(CartHandlerTest, ViewCartEmpty) {
    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/cart", "GET", ""))
        .WillOnce(::testing::Return("[]"));

    testing::internal::CaptureStdout();
    cartHandler->viewCart();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Cart: []\n");
}

TEST_F(CartHandlerTest, ViewCartFailure) {
    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/cart", "GET", ""))
        .WillOnce(::testing::Return(""));

    testing::internal::CaptureStdout();
    cartHandler->viewCart();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Cart: \n");
}