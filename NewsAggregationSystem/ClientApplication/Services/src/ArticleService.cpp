#include "../inc/ArticleService.h"
#include <ctime>
#include <sstream>

ArticleService::ArticleService(Client& c) : client(c) {}

std::vector<nlohmann::json> ArticleService::getArticles(const std::string& startDate, const std::string& endDate, const std::string& category) {
    std::string endpoint = "/news?start=" + startDate + "&end=" + endDate + "&category=" + category;
    std::string response = client.get(endpoint);

    try {
        return nlohmann::json::parse(response);
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
