#include "services/inc/SavedArticleService.h"
#include "repositories/inc/SavedArticleRepository.h"
#include <iostream>

SavedArticleService::SavedArticleService(std::shared_ptr<DBConnection> dbConn) : db(std::move(dbConn)) {}

bool SavedArticleService::saveArticle(int userId, int articleId) {
    std::cout << "[SavedArticleService] saveArticle called" << std::endl;
    try {
        SavedArticleRepository repo(db);
        bool result = repo.saveArticleForUser(userId, articleId);
        std::cout << "[SavedArticleService] saveArticle success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[SavedArticleService] saveArticle error: " << e.what() << std::endl;
        return false;
    }
}

std::vector<NewsArticle> SavedArticleService::getSavedArticles(int userId) {
    std::cout << "[SavedArticleService] getSavedArticles called" << std::endl;
    try {
        SavedArticleRepository repo(db);
        auto result = repo.getSavedArticlesByUser(userId);
        std::cout << "[SavedArticleService] getSavedArticles success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[SavedArticleService] getSavedArticles error: " << e.what() << std::endl;
        return {};
    }
}

bool SavedArticleService::deleteSavedArticle(int userId, int articleId) {
    std::cout << "[SavedArticleService] deleteSavedArticle called" << std::endl;
    try {
        SavedArticleRepository repo(db);
        bool result = repo.deleteSavedArticle(userId, articleId);
        std::cout << "[SavedArticleService] deleteSavedArticle success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[SavedArticleService] deleteSavedArticle error: " << e.what() << std::endl;
        return false;
    }
}
