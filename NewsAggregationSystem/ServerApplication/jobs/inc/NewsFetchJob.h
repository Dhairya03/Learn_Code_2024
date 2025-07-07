#ifndef NEWSFETCHJOB_H
#define NEWSFETCHJOB_H

#include "repositories/inc/NewsArticleRepository.h"
#include "services/inc/NotificationService.h"
#include "lib/json/json.hpp"
#include <memory>

class NewsFetchJob {
public:
    NewsFetchJob(std::shared_ptr<DBConnection> dbConn);
    void run();

private:
    NewsArticleRepository repository;
    std::shared_ptr<DBConnection> db;
    std::unique_ptr<NotificationService> notificationService;
    
    std::string safeGetString(const nlohmann::json& j, const std::string& key, const std::string& defaultValue = "");
    void parseAndSaveArticles(const std::string& jsonResponse);
    std::string fetchNewsFromAPI();
    void processArticleForNotifications(const NewsArticle& article);
};

#endif