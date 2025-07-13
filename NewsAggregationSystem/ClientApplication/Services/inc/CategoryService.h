#pragma once
#include "../../Core/inc/Client.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class CategoryService {
public:
    CategoryService(Client& client);

    std::vector<std::string> getAllCategories();
    
    std::string getCategoryById(int categoryId);
    
    int getCategoryIdByName(const std::string& categoryName);

private:
    Client& client;
}; 