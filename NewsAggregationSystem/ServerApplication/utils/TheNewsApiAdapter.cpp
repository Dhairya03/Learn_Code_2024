#include "TheNewsApiAdapter.h"
#include "lib/httplib/httplib.h"
#include "lib/json/json.hpp"
#include <iostream>

using json = nlohmann::json;

TheNewsApiAdapter::TheNewsApiAdapter(const std::string& apiKey) : apiKey(apiKey) {
    std::cout << "[TheNewsApiAdapter] Constructed with API key: " << (apiKey.empty() ? "<empty>" : "<hidden>") << std::endl;
}

std::vector<NewsArticle> TheNewsApiAdapter::fetchTopHeadlines() {
    std::cout << "[TheNewsApiAdapter] fetchTopHeadlines() called." << std::endl;
    std::vector<NewsArticle> articles;
    try {
        httplib::Client cli("https://api.thenewsapi.com");
        cli.set_connection_timeout(30);
        cli.set_read_timeout(30);
        std::string path = "/v1/news/top?api_token=" + apiKey + "&locale=us&limit=3";
        std::cout << "[TheNewsApiAdapter] Making request to: " << path << std::endl;
        auto res = cli.Get(path.c_str());
        if (res && res->status == 200) {
            std::cout << "[TheNewsApiAdapter] API request successful. Parsing response..." << std::endl;
            auto jsonResp = json::parse(res->body);
            if (jsonResp.contains("data") && jsonResp["data"].is_array()) {
                for (const auto& articleJson : jsonResp["data"]) {
                    NewsArticle article;
                    article.title = articleJson.value("title", "No Title");
                    article.description = articleJson.value("description", "");
                    article.url = articleJson.value("url", "");
                    article.source = articleJson.value("source", "Unknown Source");
                    article.publishedAt = articleJson.value("published_at", "");
                    article.categoryId = 7; // Default or map as needed
                    articles.push_back(article);
                }
                std::cout << "[TheNewsApiAdapter] Parsed " << articles.size() << " articles." << std::endl;
            } else {
                std::cerr << "[TheNewsApiAdapter] No 'data' array in response." << std::endl;
            }
        } else {
            std::cerr << "[TheNewsApiAdapter] API request failed or returned non-200 status." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[TheNewsApiAdapter] Exception: " << e.what() << std::endl;
    }
    std::cout << "[TheNewsApiAdapter] fetchTopHeadlines() finished." << std::endl;
    return articles;
} 