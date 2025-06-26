#include "services/inc/SavedArticleService.h"
#include "repositories/inc/SavedArticleRepository.h"

SavedArticleService::SavedArticleService(std::shared_ptr<DBConnection> dbConn) : db(std::move(dbConn)) {}

bool SavedArticleService::saveArticle(int userId, int articleId) {
    SavedArticleRepository repo(db);
    return repo.saveArticleForUser(userId, articleId);
}

std::vector<NewsArticle> SavedArticleService::getSavedArticles(int userId) {
    SavedArticleRepository repo(db);
    return repo.getSavedArticlesByUser(userId);
}

bool SavedArticleService::deleteSavedArticle(int userId, int articleId) {
    SavedArticleRepository repo(db);
    return repo.deleteSavedArticle(userId, articleId);
}
