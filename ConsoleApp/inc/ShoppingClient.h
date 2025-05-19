#ifndef SHOPPING_CLIENT_H
#define SHOPPING_CLIENT_H

#include "HttpClient.h"
#include <string>

class ShoppingClient {
public:
    void browseCategories();
    void browseProducts(const std::string& category);
    void addToCart(int productId);
};

#endif