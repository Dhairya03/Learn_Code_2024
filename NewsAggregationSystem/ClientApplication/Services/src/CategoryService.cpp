#include "../inc/CategoryService.h"
#include <iostream>

CategoryService::CategoryService(Client& c) : client(c) {}

std::vector<std::string> CategoryService::getAllCategories() {
    std::vector<std::string> categories;
    try {
        std::string response = client.get("/categories");
        auto responseJson = json::parse(response);
        
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data")) {
            auto data = responseJson["data"];
            if (data.is_array()) {
                for (const auto& category : data) {
                    if (category.contains("name")) {
                        categories.push_back(category["name"].get<std::string>());
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting categories: " << e.what() << std::endl;
        categories = {"business", "entertainment", "sports", "technology", "general"};
    }
    return categories;
}

std::string CategoryService::getCategoryById(int categoryId) {
    try {
        std::string endpoint = "/categories/" + std::to_string(categoryId);
        std::string response = client.get(endpoint);
        
        auto responseJson = json::parse(response);
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data") && responseJson["data"].contains("name")) {
            return responseJson["data"]["name"].get<std::string>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting category by ID: " << e.what() << std::endl;
    }
    return "";
}

int CategoryService::getCategoryIdByName(const std::string& categoryName) {
    try {
        std::string endpoint = "/categories/name/" + categoryName;
        std::string response = client.get(endpoint);
        
        auto responseJson = json::parse(response);
        if (responseJson.contains("success") && responseJson["success"].get<bool>() && 
            responseJson.contains("data") && responseJson["data"].contains("id")) {
            return responseJson["data"]["id"].get<int>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting category ID by name: " << e.what() << std::endl;
    }
    return -1;
} 