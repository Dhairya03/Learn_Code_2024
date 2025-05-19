#include "ShoppingClient.h"
#include <iostream>

void ShoppingClient::browseCategories() {
    std::string response = HttpClient::sendRequest("http://localhost:8080/categories", "GET");
    std::cout << "Categories: " << response << std::endl;
}

void ShoppingClient::browseProducts(const std::string& category) {
    std::string response = HttpClient::sendRequest("http://localhost:8080/products?category=" + category, "GET");
    std::cout << "Products: " << response << std::endl;
}

void ShoppingClient::addToCart(int productId) {
    std::string response = HttpClient::sendRequest("http://localhost:8080/cart/add", "POST", "{\"product_id\": " + std::to_string(productId) + "}");
    std::cout << "Response: " << response << std::endl;
}
