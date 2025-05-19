#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ProductHandler.h"
#include <mysql/mysql.h>
#include "MockMySql.h"

class ProductHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockMYSQL = new MockMYSQL();
        productHandler = new ProductHandler(reinterpret_cast<MYSQL*>(mockMYSQL));
    }

    void TearDown() override {
        delete productHandler;
        delete mockMYSQL;
    }

    MockMYSQL* mockMYSQL;
    ProductHandler* productHandler;
};

TEST_F(ProductHandlerTest, HandleGetCategoriesSuccess) {
    MYSQL_RES mockRes;
    MYSQL_ROW mockRow1 = new char*[1]{(char*)"Electronics"};
    MYSQL_ROW mockRow2 = new char*[1]{(char*)"Clothing"};

    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockMYSQL, mysql_store_result(::testing::_))
        .WillOnce(::testing::Return(&mockRes));
    EXPECT_CALL(*mockMYSQL, mysql_fetch_row(&mockRes))
        .WillOnce(::testing::Return(mockRow1))
        .WillOnce(::testing::Return(mockRow2))
        .WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(*mockMYSQL, mysql_free_result(&mockRes));

    std::string result = productHandler->handleGetCategories();
    EXPECT_EQ(result, R"(["Electronics", "Clothing"])");

    delete[] mockRow1;
    delete[] mockRow2;
}

TEST_F(ProductHandlerTest, HandleGetCategoriesEmpty) {
    MYSQL_RES mockRes;

    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockMYSQL, mysql_store_result(::testing::_))
        .WillOnce(::testing::Return(&mockRes));
    EXPECT_CALL(*mockMYSQL, mysql_fetch_row(&mockRes))
        .WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(*mockMYSQL, mysql_free_result(&mockRes));

    std::string result = productHandler->handleGetCategories();
    EXPECT_EQ(result, "[]");
}

TEST_F(ProductHandlerTest, HandleGetProductsSuccess) {
    MYSQL_RES mockRes;
    MYSQL_ROW mockRow1 = new char*[2]{(char*)"1", (char*)"Laptop"};
    MYSQL_ROW mockRow2 = new char*[2]{(char*)"2", (char*)"T-Shirt"};

    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockMYSQL, mysql_store_result(::testing::_))
        .WillOnce(::testing::Return(&mockRes));
    EXPECT_CALL(*mockMYSQL, mysql_fetch_row(&mockRes))
        .WillOnce(::testing::Return(mockRow1))
        .WillOnce(::testing::Return(mockRow2))
        .WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(*mockMYSQL, mysql_free_result(&mockRes));

    std::string result = productHandler->handleGetProducts();
    EXPECT_EQ(result, R"([{"id":1, "name":"Laptop"}, {"id":2, "name":"T-Shirt"}])");

    delete[] mockRow1;
    delete[] mockRow2;
}

TEST_F(ProductHandlerTest, HandleGetProductsEmpty) {
    MYSQL_RES mockRes;

    EXPECT_CALL(*mockMYSQL, mysql_query(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockMYSQL, mysql_store_result(::testing::_))
        .WillOnce(::testing::Return(&mockRes));
    EXPECT_CALL(*mockMYSQL, mysql_fetch_row(&mockRes))
        .WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(*mockMYSQL, mysql_free_result(&mockRes));

    std::string result = productHandler->handleGetProducts();
    EXPECT_EQ(result, "[]");
}