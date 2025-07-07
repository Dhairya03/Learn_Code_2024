#include "services/inc/NewsService.h"
#include "repositories/inc/NewsArticleRepository.h"

NewsService::NewsService(std::shared_ptr<DBConnection> dbConn) : db(std::move(dbConn)) {}

std::vector<NewsArticle> NewsService::getAllArticles()
{
    NewsArticleRepository repo(db);
    return repo.getAllArticles();
}

std::vector<NewsArticle> NewsService::getArticlesByCategory(const std::string &category)
{
    NewsArticleRepository repo(db);
    return repo.getArticlesByCategory(category);
}

std::vector<NewsArticle> NewsService::searchArticles(const std::string& query, const std::string& startDate, const std::string& endDate, const std::string& sort) {
    NewsArticleRepository repo(db);
    return repo.searchArticles(query, startDate, endDate, sort);
}

std::vector<NewsArticle> NewsService::getNewsByDateAndCategory(const std::string &startDate, const std::string &endDate, const std::string &category)
{
    NewsArticleRepository repo(db);
    return repo.getArticlesByDateAndCategory(startDate, endDate, category);
}

std::vector<NewsArticle> NewsService::getArticlesToday()
{
    NewsArticleRepository repo(db);
    return repo.getArticlesToday();
}