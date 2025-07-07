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
    std::vector<nlohmann::json> searchArticles(const std::string& query, const std::string& startDate, const std::string& endDate, const std::string& sort);

private:
    Client& client;
};
