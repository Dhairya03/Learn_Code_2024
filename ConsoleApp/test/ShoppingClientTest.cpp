#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ShoppingClient.h"
#include "HttpClient.h"
#include "MockHttpClient.h"

class ShoppingClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockHttpClient = new MockHttpClient();
        shoppingClient = new ShoppingClient();
        HttpClient::setMockInstance(mockHttpClient);
    }

    void TearDown() override {
        delete shoppingClient;
        delete mockHttpClient;
    }

    MockHttpClient* mockHttpClient;
    ShoppingClient* shoppingClient;
};

TEST_F(ShoppingClientTest, BrowseCategoriesSuccess) {
    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/categories", "GET", ""))
        .WillOnce(::testing::Return(R"(["Electronics", "Clothing"])"));

    testing::internal::CaptureStdout();
    shoppingClient->browseCategories();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Categories: [\"Electronics\", \"Clothing\"]\n");
}

TEST_F(ShoppingClientTest, BrowseProductsSuccess) {
    std::string category = "Electronics";

    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/products?category=Electronics", "GET", ""))
        .WillOnce(::testing::Return(R"([{"id":1, "name":"Laptop"}, {"id":2, "name":"Smartphone"}])"));

    testing::internal::CaptureStdout();
    shoppingClient->browseProducts(category);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Products: [{\"id\":1, \"name\":\"Laptop\"}, {\"id\":2, \"name\":\"Smartphone\"}]\n");
}

TEST_F(ShoppingClientTest, AddToCartSuccess) {
    int productId = 101;

    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/cart/add", "POST", "{\"product_id\": 101}"))
        .WillOnce(::testing::Return("Product added to cart"));

    testing::internal::CaptureStdout();
    shoppingClient->addToCart(productId);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Response: Product added to cart\n");
}

TEST_F(ShoppingClientTest, AddToCartFailure) {
    int productId = 101;

    EXPECT_CALL(*mockHttpClient, sendRequest("http://localhost:8080/cart/add", "POST", "{\"product_id\": 101}"))
        .WillOnce(::testing::Return("Failed to add product to cart"));

    testing::internal::CaptureStdout();
    shoppingClient->addToCart(productId);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Response: Failed to add product to cart\n");
}