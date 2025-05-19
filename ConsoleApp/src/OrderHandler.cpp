#include "OrderHandler.h"
#include <iostream>

void OrderHandler::handleOrderHistory() {
    std::string response = HttpClient::sendRequest("http://localhost:8080/orders", "GET");
    std::cout << "Order History: " << response << std::endl;
}