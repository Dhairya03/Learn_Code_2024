#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "OrderHandler.h"
#include <mysql/mysql.h>
#include "MockMySql.h"

class OrderHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockMYSQL = new MockMYSQL();
        orderHandler = new OrderHandler(reinterpret_cast<MYSQL*>(mockMYSQL));
    }

    void TearDown() override {
        delete orderHandler;
        delete mockMYSQL;
    }

    MockMYSQL* mockMYSQL;
    OrderHandler* orderHandler;
};

TEST_F(OrderHandlerTest, HandleOrderHistorySuccess) {
    MYSQL_RES mockRes;
    MYSQL_ROW mockRow1 = new char*[3]{(char*)"1", (char*)"101", (char*)"2"};
    MYSQL_ROW mockRow2 = new char*[3]{(char*)"2", (char*)"102", (char*)"1"};

    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockMYSQL, mysql_store_result(::testing::_))
        .WillOnce(::testing::Return(&mockRes));
    EXPECT_CALL(*mockMYSQL, mysql_fetch_row(&mockRes))
        .WillOnce(::testing::Return(mockRow1))
        .WillOnce(::testing::Return(mockRow2))
        .WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(*mockMYSQL, mysql_free_result(&mockRes));

    std::string result = orderHandler->handleOrderHistory();
    EXPECT_EQ(result, R"([{"order_id":1, "product_id": 101, "quantity": 2}, {"order_id":2, "product_id": 102, "quantity": 1}])");

    delete[] mockRow1;
    delete[] mockRow2;
}

TEST_F(OrderHandlerTest, HandleOrderHistoryEmpty) {
    MYSQL_RES mockRes;

    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockMYSQL, mysql_store_result(::testing::_))
        .WillOnce(::testing::Return(&mockRes));
    EXPECT_CALL(*mockMYSQL, mysql_fetch_row(&mockRes))
        .WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(*mockMYSQL, mysql_free_result(&mockRes));

    std::string result = orderHandler->handleOrderHistory();
    EXPECT_EQ(result, "[]");
}

TEST_F(OrderHandlerTest, HandleOrderHistoryQueryFailure) {
    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(1));

    std::string result = orderHandler->handleOrderHistory();
    EXPECT_EQ(result, "[]");
}