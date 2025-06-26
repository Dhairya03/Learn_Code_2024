#pragma once
#include "../../Core/inc/Client.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class ArticleService {
public:
    explicit ArticleService(Client& client);
    std::vector<nlohmann::json> getArticles(const std::string& startDate, const std::string& endDate, const std::string& category);
    static std::string getTodayDate();

private:
    Client& client;
};
