#include "NewsApiOrgAdapter.h"
#include "lib/httplib/httplib.h"
#include "lib/json/json.hpp"
#include <iostream>

using json = nlohmann::json;

NewsApiOrgAdapter::NewsApiOrgAdapter(const std::string& apiKey) : apiKey(apiKey) {
    std::cout << "[NewsApiOrgAdapter] Constructed with API key: " << (apiKey.empty() ? "<empty>" : "<hidden>") << std::endl;
}

std::vector<NewsArticle> NewsApiOrgAdapter::fetchTopHeadlines() {
    std::cout << "[NewsApiOrgAdapter] fetchTopHeadlines() called." << std::endl;
    std::vector<NewsArticle> articles;
    try {
        httplib::Client cli("https://newsapi.org");
        cli.set_connection_timeout(30);
        cli.set_read_timeout(30);
        std::string path = "/v2/top-headlines?country=us&apiKey=" + apiKey;
        std::cout << "[NewsApiOrgAdapter] Making request to: " << path << std::endl;
        auto res = cli.Get(path.c_str());
        if (res && res->status == 200) {
            std::cout << "[NewsApiOrgAdapter] API request successful. Parsing response..." << std::endl;
            auto jsonResp = json::parse(res->body);
            if (jsonResp.contains("articles") && jsonResp["articles"].is_array()) {
                for (const auto& articleJson : jsonResp["articles"]) {
                    NewsArticle article;
                    article.title = articleJson.value("title", "No Title");
                    article.description = articleJson.value("description", "");
                    article.url = articleJson.value("url", "");
                    if (articleJson.contains("source") && articleJson["source"].is_object()) {
                        article.source = articleJson["source"].value("name", "Unknown Source");
                    } else {
                        article.source = "Unknown Source";
                    }
                    article.publishedAt = articleJson.value("publishedAt", "");
                    article.categoryId = 7; // Default or map as needed
                    articles.push_back(article);
                }
                std::cout << "[NewsApiOrgAdapter] Parsed " << articles.size() << " articles." << std::endl;
            } else {
                std::cerr << "[NewsApiOrgAdapter] No 'articles' array in response." << std::endl;
            }
        } else {
            std::cerr << "[NewsApiOrgAdapter] API request failed or returned non-200 status." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[NewsApiOrgAdapter] Exception: " << e.what() << std::endl;
    }
    std::cout << "[NewsApiOrgAdapter] fetchTopHeadlines() finished." << std::endl;
    return articles;
} 