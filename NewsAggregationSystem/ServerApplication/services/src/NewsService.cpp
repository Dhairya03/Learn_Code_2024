#include "services/inc/NewsService.h"
#include "repositories/inc/NewsArticleRepository.h"
#include <iostream>

NewsService::NewsService(std::shared_ptr<DBConnection> dbConn) : db(std::move(dbConn)) {}

std::vector<NewsArticle> NewsService::getAllArticles()
{
    std::cout << "[NewsService] getAllArticles called" << std::endl;
    try {
        NewsArticleRepository repo(db);
        auto result = repo.getAllArticles();
        std::cout << "[NewsService] getAllArticles success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NewsService] getAllArticles error: " << e.what() << std::endl;
        return {};
    }
}

std::vector<NewsArticle> NewsService::getArticlesByCategory(const std::string &category)
{
    std::cout << "[NewsService] getArticlesByCategory called" << std::endl;
    try {
        NewsArticleRepository repo(db);
        auto result = repo.getArticlesByCategory(category);
        std::cout << "[NewsService] getArticlesByCategory success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NewsService] getArticlesByCategory error: " << e.what() << std::endl;
        return {};
    }
}

std::vector<NewsArticle> NewsService::searchArticles(const std::string& query, const std::string& startDate, const std::string& endDate, const std::string& sort) {
    std::cout << "[NewsService] searchArticles called" << std::endl;
    try {
        NewsArticleRepository repo(db);
        auto result = repo.searchArticles(query, startDate, endDate, sort);
        std::cout << "[NewsService] searchArticles success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NewsService] searchArticles error: " << e.what() << std::endl;
        return {};
    }
}

std::vector<NewsArticle> NewsService::getNewsByDateAndCategory(const std::string &startDate, const std::string &endDate, const std::string &category)
{
    std::cout << "[NewsService] getNewsByDateAndCategory called" << std::endl;
    try {
        NewsArticleRepository repo(db);
        auto result = repo.getArticlesByDateAndCategory(startDate, endDate, category);
        std::cout << "[NewsService] getNewsByDateAndCategory success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NewsService] getNewsByDateAndCategory error: " << e.what() << std::endl;
        return {};
    }
}

std::vector<NewsArticle> NewsService::getArticlesToday()
{
    std::cout << "[NewsService] getArticlesToday called" << std::endl;
    try {
        NewsArticleRepository repo(db);
        auto result = repo.getArticlesToday();
        std::cout << "[NewsService] getArticlesToday success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[NewsService] getArticlesToday error: " << e.what() << std::endl;
        return {};
    }
}