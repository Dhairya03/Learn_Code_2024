#ifndef NEWSFETCHJOB_H
#define NEWSFETCHJOB_H

#include "repositories/inc/NewsArticleRepository.h"
#include "lib/json/json.hpp"
#include <memory>

class NewsFetchJob {
public:
    NewsFetchJob(std::shared_ptr<DBConnection> dbConn);
    void run();

private:
    NewsArticleRepository repository;
    std::shared_ptr<DBConnection> db;
    
    std::string safeGetString(const nlohmann::json& j, const std::string& key, const std::string& defaultValue = "");
    void parseAndSaveArticles(const std::string& jsonResponse);
    std::string fetchNewsFromAPI();
};

#endif