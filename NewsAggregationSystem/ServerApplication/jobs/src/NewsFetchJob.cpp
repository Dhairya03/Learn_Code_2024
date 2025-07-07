#include "jobs/inc/NewsFetchJob.h"
#include "repositories/inc/NewsArticleRepository.h"
#include "services/inc/NotificationService.h"
#include "lib/httplib/httplib.h"
#include "lib/json/json.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using json = nlohmann::json;

NewsFetchJob::NewsFetchJob(std::shared_ptr<DBConnection> dbConn, std::unique_ptr<INewsApiAdapter> apiAdapter)
    : repository(dbConn), db(dbConn), notificationService(std::make_unique<NotificationService>(dbConn)), apiAdapter(std::move(apiAdapter)) {}

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

void NewsFetchJob::parseAndSaveArticles(const std::vector<NewsArticle>& articles) {
    if (articles.empty()) {
        std::cerr << "[NewsFetchJob] No articles to process\n";
        return;
    }
    std::cout << "[NewsFetchJob] Found " << articles.size() << " articles to process\n";
    int successCount = 0;
    int skipCount = 0;
    for (size_t i = 0; i < articles.size(); ++i) {
        const auto& article = articles[i];
        std::cout << "[NewsFetchJob] Processing article " << (i + 1) << "/" << articles.size() << "\n";
        if (article.title == "No Title" || article.url.empty()) {
            std::cout << "[NewsFetchJob] Skipping article " << (i + 1) << " - missing essential data\n";
            skipCount++;
            continue;
        }
        if (repository.saveArticle(article)) {
            successCount++;
            std::cout << "[NewsFetchJob] ✓ Article " << (i + 1) << " saved successfully\n";
            processArticleForNotifications(article);
        } else {
            std::cout << "[NewsFetchJob] ✗ Failed to save article " << (i + 1) << "\n";
        }
    }
    std::cout << "[NewsFetchJob] Processing complete. Saved: " << successCount 
              << ", Skipped: " << skipCount << std::endl;
}

void NewsFetchJob::run() {
    try {
        std::cout << "[NewsFetchJob] Fetching news articles...\n";
        auto articles = apiAdapter->fetchTopHeadlines();
        if (articles.empty()) {
            std::cout << "[NewsFetchJob] No articles fetched from API\n";
            return;
        }
        parseAndSaveArticles(articles);
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
