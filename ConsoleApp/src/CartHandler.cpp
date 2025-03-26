#include "CartHandler.h"
#include <iostream>

void CartHandler::viewCart() {
    std::string response = HttpClient::sendRequest("http://localhost:8080/cart", "GET");
    std::cout << "Cart: " << response << std::endl;
}