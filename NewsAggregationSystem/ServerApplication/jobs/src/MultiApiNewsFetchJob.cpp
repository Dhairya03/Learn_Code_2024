#include "jobs/inc/MultiApiNewsFetchJob.h"
#include "repositories/inc/NewsArticleRepository.h"
#include "services/inc/NotificationService.h"
#include "repositories/inc/ExternalServerRepository.h"
#include "utils/NewsApiAdapterFactory.h"
#include "utils/NewsApiOrgAdapter.h"
#include "utils/TheNewsApiAdapter.h"
#include "lib/json/json.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <algorithm>

using json = nlohmann::json;

MultiApiNewsFetchJob::MultiApiNewsFetchJob(std::shared_ptr<DBConnection> dbConn)
    : db(dbConn), 
      repository(std::make_unique<NewsArticleRepository>(dbConn)),
      notificationService(std::make_unique<NotificationService>(dbConn)),
      externalServerRepo(std::make_unique<ExternalServerRepository>(dbConn)) {}

std::string MultiApiNewsFetchJob::safeGetString(const nlohmann::json& j, const std::string& key, const std::string& defaultValue) {
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
        std::cerr << "[MultiApiNewsFetchJob] Error extracting field '" << key << "': " << e.what() << std::endl;
        return defaultValue;
    }
}

void MultiApiNewsFetchJob::parseAndSaveArticles(const std::vector<NewsArticle>& articles, const std::string& sourceName) {
    if (articles.empty()) {
        std::cerr << "[MultiApiNewsFetchJob] No articles to process from " << sourceName << "\n";
        return;
    }
    std::cout << "[MultiApiNewsFetchJob] Found " << articles.size() << " articles from " << sourceName << " to process\n";
    int successCount = 0;
    int skipCount = 0;
    for (size_t i = 0; i < articles.size(); ++i) {
        const auto& article = articles[i];
        std::cout << "[MultiApiNewsFetchJob] Processing article " << (i + 1) << "/" << articles.size() << " from " << sourceName << "\n";
        if (article.title == "No Title" || article.url.empty()) {
            std::cout << "[MultiApiNewsFetchJob] Skipping article " << (i + 1) << " from " << sourceName << " - missing essential data\n";
            skipCount++;
            continue;
        }
        if (repository->saveArticle(article)) {
            successCount++;
            std::cout << "[MultiApiNewsFetchJob] ✓ Article " << (i + 1) << " from " << sourceName << " saved successfully\n";
            processArticleForNotifications(article);
        } else {
            std::cout << "[MultiApiNewsFetchJob] ✗ Failed to save article " << (i + 1) << " from " << sourceName << "\n";
        }
    }
    std::cout << "[MultiApiNewsFetchJob] Processing complete for " << sourceName << ". Saved: " << successCount 
              << ", Skipped: " << skipCount << std::endl;
}

void MultiApiNewsFetchJob::processArticleForNotifications(const NewsArticle& article) {
    try {
        std::cout << "[MultiApiNewsFetchJob] Processing article for notifications: " << article.title << std::endl;
        notificationService->processNewsArticleForNotifications(article);
    } catch (const std::exception& e) {
        std::cerr << "[MultiApiNewsFetchJob] Error processing article for notifications: " << e.what() << std::endl;
    }
}

std::vector<NewsArticle> MultiApiNewsFetchJob::fetchFromSingleApi(std::unique_ptr<INewsApiAdapter> adapter, const std::string& sourceName) {
    try {
        std::cout << "[MultiApiNewsFetchJob] 🔄 Starting fetch from " << sourceName << "..." << std::endl;
        auto startTime = std::chrono::high_resolution_clock::now();
        
        auto articles = adapter->fetchTopHeadlines();
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        std::cout << "[MultiApiNewsFetchJob] ✅ Fetched " << articles.size() << " articles from " << sourceName 
                  << " in " << duration.count() << "ms" << std::endl;
        return articles;
    } catch (const std::exception& e) {
        std::cerr << "[MultiApiNewsFetchJob] ❌ Error fetching from " << sourceName << ": " << e.what() << std::endl;
        return std::vector<NewsArticle>();
    }
}

void MultiApiNewsFetchJob::run() {
    try {
        std::cout << "\n[MultiApiNewsFetchJob] ========================================" << std::endl;
        std::cout << "[MultiApiNewsFetchJob] Starting multi-API news fetch..." << std::endl;
        std::cout << "[MultiApiNewsFetchJob] ========================================" << std::endl;
        
        auto servers = externalServerRepo->getAllServerDetails();
        std::vector<ExternalServer> activeServers;
        
        std::cout << "[MultiApiNewsFetchJob] Checking " << servers.size() << " configured servers..." << std::endl;
        for (const auto& server : servers) {
            std::cout << "[MultiApiNewsFetchJob] Server: " << server.name 
                      << " (Active: " << (server.isActive ? "YES" : "NO")
                      << ", Has API Key: " << (!server.apiKey.empty() ? "YES" : "NO") << ")" << std::endl;
            
            if (server.isActive && !server.apiKey.empty()) {
                activeServers.push_back(server);
                std::cout << "[MultiApiNewsFetchJob] ✓ Added to active list: " << server.name << std::endl;
            } else {
                std::cout << "[MultiApiNewsFetchJob] ✗ Skipped: " << server.name 
                          << " (inactive or missing API key)" << std::endl;
            }
        }
        
        if (activeServers.empty()) {
            std::cout << "[MultiApiNewsFetchJob] ⚠️  No active servers found, using default NewsAPI.org" << std::endl;
            auto defaultAdapter = std::make_unique<NewsApiOrgAdapter>("8f4cda27870e4de79de71ec7876d3733");
            auto articles = fetchFromSingleApi(std::move(defaultAdapter), "NewsAPI.org (default)");
            parseAndSaveArticles(articles, "NewsAPI.org (default)");
            return;
        }
        
        std::cout << "[MultiApiNewsFetchJob] 🚀 Starting parallel fetch from " << activeServers.size() << " APIs..." << std::endl;
        
        std::vector<std::future<std::vector<NewsArticle>>> futures;
        std::vector<std::string> sourceNames;
        
        for (const auto& server : activeServers) {
            std::cout << "[MultiApiNewsFetchJob] Creating adapter for: " << server.name << std::endl;
            auto adapter = NewsApiAdapterFactory::createAdapter(server);
            if (adapter) {
                futures.push_back(std::async(std::launch::async, 
                    [this, adapter = std::move(adapter), server]() mutable {
                        return fetchFromSingleApi(std::move(adapter), server.name);
                    }));
                sourceNames.push_back(server.name);
                std::cout << "[MultiApiNewsFetchJob] ✓ Scheduled fetch for: " << server.name << std::endl;
            } else {
                std::cout << "[MultiApiNewsFetchJob] ✗ Failed to create adapter for: " << server.name << std::endl;
            }
        }
        
        std::cout << "[MultiApiNewsFetchJob] Waiting for " << futures.size() << " API calls to complete..." << std::endl;
        
        for (size_t i = 0; i < futures.size(); ++i) {
            try {
                std::cout << "[MultiApiNewsFetchJob] Processing results from: " << sourceNames[i] << std::endl;
                auto articles = futures[i].get();
                parseAndSaveArticles(articles, sourceNames[i]);
            } catch (const std::exception& e) {
                std::cerr << "[MultiApiNewsFetchJob] Error processing results from " << sourceNames[i] << ": " << e.what() << std::endl;
            }
        }
        
        std::cout << "[MultiApiNewsFetchJob] ✅ Multi-API fetch completed successfully" << std::endl;
        std::cout << "[MultiApiNewsFetchJob] ========================================" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "[MultiApiNewsFetchJob] ❌ Error in run(): " << e.what() << std::endl;
    }
} 