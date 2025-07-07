#include "jobs/inc/NewsFetchJob.h"
#include "repositories/inc/NewsArticleRepository.h"
#include "services/inc/NotificationService.h"
#include "lib/httplib/httplib.h"
#include "lib/json/json.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using json = nlohmann::json;

NewsFetchJob::NewsFetchJob(std::shared_ptr<DBConnection> dbConn) 
    : repository(dbConn), db(dbConn), notificationService(std::make_unique<NotificationService>(dbConn)) {}

std::string NewsFetchJob::safeGetString(const nlohmann::json& j, const std::string& key, const std::string& defaultValue) {
    try {
        if (j.contains(key)) {
            if (j[key].is_null()) {
                return defaultValue;
            }
            if (j[key].is_string()) {
                return j[key].get<std::string>();
            }
            if (j[key].is_number() || j[key].is_boolean()) {
                return j[key].dump();
            }
        }
        return defaultValue;
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "[NewsFetchJob] Error extracting field '" << key << "': " << e.what() << std::endl;
        return defaultValue;
    }
}

void NewsFetchJob::parseAndSaveArticles(const std::string& jsonResponse) {
    if (jsonResponse.empty()) {
        std::cerr << "[NewsFetchJob] Empty JSON response\n";
        return;
    }

    try {
        std::cout << "[NewsFetchJob] Parsing JSON response...\n";
        auto json = nlohmann::json::parse(jsonResponse);
        
        if (!json.contains("articles")) {
            std::cerr << "[NewsFetchJob] JSON response missing 'articles' field\n";
            return;
        }

        if (!json["articles"].is_array()) {
            std::cerr << "[NewsFetchJob] 'articles' field is not an array\n";
            return;
        }
        
        auto articles = json["articles"];
        std::cout << "[NewsFetchJob] Found " << articles.size() << " articles to process\n";
        
        int successCount = 0;
        int skipCount = 0;
        
        for (size_t i = 0; i < articles.size(); ++i) {
            try {
                const auto& articleJson = articles[i];
                NewsArticle article;
                
                std::cout << "[NewsFetchJob] Processing article " << (i + 1) << "/" << articles.size() << "\n";
                
                article.title = safeGetString(articleJson, "title", "No Title");
                article.description = safeGetString(articleJson, "description", "");
                article.url = safeGetString(articleJson, "url", "");
                
                std::string source = "";
                if (articleJson.contains("source")) {
                    if (articleJson["source"].is_object() && articleJson["source"].contains("name")) {
                        source = safeGetString(articleJson["source"], "name", "Unknown Source");
                    } else if (articleJson["source"].is_string()) {
                        source = safeGetString(articleJson, "source", "Unknown Source");
                    } else {
                        source = "Unknown Source";
                    }
                } else {
                    source = "Unknown Source";
                }
                article.source = source;
                
                article.publishedAt = safeGetString(articleJson, "publishedAt", "");
                article.categoryId = 7; 

                if (article.title == "No Title" || article.url.empty()) {
                    std::cout << "[NewsFetchJob] Skipping article " << (i + 1) << " - missing essential data\n";
                    skipCount++;
                    continue;
                }
                
                if (repository.saveArticle(article)) {
                    successCount++;
                    std::cout << "[NewsFetchJob] ✓ Article " << (i + 1) << " saved successfully\n";
                    
                    // Process article for notifications
                    processArticleForNotifications(article);
                } else {
                    std::cout << "[NewsFetchJob] ✗ Failed to save article " << (i + 1) << "\n";
                }
                
            } catch (const nlohmann::json::exception& e) {
                std::cerr << "[NewsFetchJob] JSON error processing article " << (i + 1) << ": " << e.what() << std::endl;
                skipCount++;
                continue;
            } catch (const std::exception& e) {
                std::cerr << "[NewsFetchJob] General error processing article " << (i + 1) << ": " << e.what() << std::endl;
                skipCount++;
                continue;
            }
        }
        
        std::cout << "[NewsFetchJob] Processing complete. Saved: " << successCount 
                  << ", Skipped: " << skipCount << std::endl;
        
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "[NewsFetchJob] JSON parse error: " << e.what() << std::endl;
        std::cerr << "[NewsFetchJob] Response preview (first 300 chars): " 
                  << jsonResponse.substr(0, 300) << "..." << std::endl;
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "[NewsFetchJob] JSON exception: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[NewsFetchJob] Unexpected error: " << e.what() << std::endl;
    }
}

std::string NewsFetchJob::fetchNewsFromAPI() {
    try {
        std::cout << "[NewsFetchJob] Connecting to News API...\n";
        
        httplib::Client cli("https://newsapi.org");
        
        cli.set_connection_timeout(30);
        cli.set_read_timeout(30);

        std::string apiKey = "8f4cda27870e4de79de71ec7876d3733";
        
        std::string path = "/v2/top-headlines?country=us&apiKey=" + apiKey;
        
        std::cout << "[NewsFetchJob] Making request to: " << path << "\n";
        
        auto res = cli.Get(path.c_str());
        
        if (res) {
            if (res->status == 200) {
                std::cout << "[NewsFetchJob] Successfully fetched data from News API\n";
                std::cout << "[NewsFetchJob] Response size: " << res->body.size() << " bytes\n";
                return res->body;
            } else {
                std::cerr << "[NewsFetchJob] API request failed with status: " << res->status << "\n";
                std::cerr << "[NewsFetchJob] Response: " << res->body << "\n";
                return "";
            }
        } else {
            auto err = res.error();
            std::cerr << "[NewsFetchJob] HTTP request failed: " << httplib::to_string(err) << "\n";
            return "";
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[NewsFetchJob] Exception in fetchNewsFromAPI: " << e.what() << "\n";
        return "";
    }
}

void NewsFetchJob::run() {
    try {
        std::cout << "[NewsFetchJob] Fetching news articles...\n";
        
        std::string jsonResponse = fetchNewsFromAPI();
        
        if (jsonResponse.empty()) {
            std::cout << "[NewsFetchJob] No response from news API\n";
            return;
        }
        
        parseAndSaveArticles(jsonResponse);   

    } catch (const std::exception& e) {
        std::cerr << "[NewsFetchJob] Error in run(): " << e.what() << std::endl;
    }
}

void NewsFetchJob::processArticleForNotifications(const NewsArticle& article) {
    try {
        std::cout << "[NewsFetchJob] Processing article for notifications: " << article.title << std::endl;
        notificationService->processNewsArticleForNotifications(article);
    } catch (const std::exception& e) {
        std::cerr << "[NewsFetchJob] Error processing article for notifications: " << e.what() << std::endl;
    }
}
