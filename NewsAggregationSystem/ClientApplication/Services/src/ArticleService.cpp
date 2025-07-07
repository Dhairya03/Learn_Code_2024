#include "../inc/ArticleService.h"
#include <ctime>
#include <sstream>

ArticleService::ArticleService(Client& c) : client(c) {}

std::vector<nlohmann::json> ArticleService::getArticles(const std::string& startDate, const std::string& endDate, const std::string& category) {
    std::string endpoint = "/news?start=" + startDate + "&end=" + endDate + "&category=" + category;
    std::string response = client.get(endpoint);

    try {
        auto parsed = nlohmann::json::parse(response);
        if (parsed.is_array()) {
            return parsed;
        } else if (parsed.is_object() && parsed.contains("data")) {
            return parsed["data"].get<std::vector<nlohmann::json>>();
        } else {
            return {};
        }
    } catch (...) {
        return {};
    }
}

std::string ArticleService::getTodayDate() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::ostringstream oss;
    oss << (now->tm_year + 1900) << "-"
        << (now->tm_mon + 1) << "-"
        << now->tm_mday;
    return oss.str();
}

std::vector<nlohmann::json> ArticleService::searchArticles(const std::string& query, const std::string& startDate, const std::string& endDate, const std::string& sort) {
    std::vector<nlohmann::json> results;
    std::string endpoint = "/news/search?q=" + query;
    if (!startDate.empty()) endpoint += "&start_date=" + startDate;
    if (!endDate.empty()) endpoint += "&end_date=" + endDate;
    if (!sort.empty()) endpoint += "&sort=" + sort;
    std::string response = client.get(endpoint);
    try {
        auto articles = nlohmann::json::parse(response);
        if (articles.is_array()) {
            for (const auto& article : articles) {
                results.push_back(article);
            }
        }
    } catch (...) {
        // Ignore parse errors for now
    }
    return results;
}
