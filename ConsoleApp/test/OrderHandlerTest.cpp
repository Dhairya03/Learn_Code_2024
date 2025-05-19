#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "OrderHandler.h"
#include "HttpClient.h"
#include "MockHttpClient.h"

class OrderHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockHttpClient = new MockHttpClient();
        orderHandler = new OrderHandler();
        HttpClient::setMockInstance(mockHttpClient); 
    }

    void TearDown() override {
        delete orderHandler;
        delete mockHttpClient;
    }

    MockHttpClient* mockHttpClient;
    OrderHandler* orderHandler;
};

TEST_F(OrderHandlerTest, HandleOrderHistorySuccess) {
    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/orders", "GET", ""))
        .WillOnce(::testing::Return(R"([{"order_id":1, "product_id":101, "quantity":2}])"));

    testing::internal::CaptureStdout();
    orderHandler->handleOrderHistory();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Order History: [{\"order_id\":1, \"product_id\":101, \"quantity\":2}]\n");
}

TEST_F(OrderHandlerTest, HandleOrderHistoryFailure) {
    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/orders", "GET", ""))
        .WillOnce(::testing::Return(""));

    testing::internal::CaptureStdout();
    orderHandler->handleOrderHistory();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Order History: \n");
}