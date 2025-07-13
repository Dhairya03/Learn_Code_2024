#ifndef MULTIAPINEWSFETCHJOB_H
#define MULTIAPINEWSFETCHJOB_H

#include "jobs/inc/NewsFetchJob.h"
#include "repositories/inc/ExternalServerRepository.h"
#include "utils/NewsApiAdapterFactory.h"
#include "utils/INewsApiAdapter.h"
#include <vector>
#include <memory>
#include <thread>
#include <future>

class MultiApiNewsFetchJob {
private:
    std::shared_ptr<DBConnection> db;
    std::unique_ptr<NewsArticleRepository> repository;
    std::unique_ptr<NotificationService> notificationService;
    std::unique_ptr<ExternalServerRepository> externalServerRepo;
    
    std::string safeGetString(const nlohmann::json& j, const std::string& key, const std::string& defaultValue);
    void parseAndSaveArticles(const std::vector<NewsArticle>& articles, const std::string& sourceName);
    void processArticleForNotifications(const NewsArticle& article);
    std::vector<NewsArticle> fetchFromSingleApi(std::unique_ptr<INewsApiAdapter> adapter, const std::string& sourceName);

public:
    MultiApiNewsFetchJob(std::shared_ptr<DBConnection> dbConn);
    void run();
};

#endif 