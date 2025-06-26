#include "repositories/inc/SavedArticleRepository.h"
#include "database/inc/DBConnection.h"
#include <cppconn/prepared_statement.h>
#include <iostream>

SavedArticleRepository::SavedArticleRepository(std::shared_ptr<DBConnection> dbConn) : db(std::move(dbConn)) {}

bool SavedArticleRepository::saveArticleForUser(int userId, int articleId) {
    if (!db || !db->isConnected()) return false;

    try {
        auto conn = db->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("INSERT INTO saved_articles (user_id, article_id) VALUES (?, ?)"));

        stmt->setInt(1, userId);
        stmt->setInt(2, articleId);
        stmt->execute();
        return true;
    } catch (const sql::SQLException& e) {
        std::cerr << "[SavedArticleRepository] Error: " << e.what() << "\n";
        return false;
    }
}

std::vector<NewsArticle> SavedArticleRepository::getSavedArticlesByUser(int userId) {
    std::vector<NewsArticle> articles;
    if (!db || !db->isConnected()) return articles;

    try {
        auto conn = db->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(
                "SELECT a.id, a.title, a.description, a.url, a.source, a.published_at "
                "FROM saved_articles sa "
                "JOIN articles a ON sa.article_id = a.id "
                "WHERE sa.user_id = ?"
            )
        );

        stmt->setInt(1, userId);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        while (res->next()) {
            NewsArticle article;
            article.id = res->getInt("id");
            article.title = res->getString("title");
            article.description = res->getString("description");
            article.url = res->getString("url");
            article.source = res->getString("source");
            article.publishedAt = res->getString("published_at");
            articles.push_back(article);
        }

    } catch (const sql::SQLException& e) {
        std::cerr << "[SavedArticleRepository] getSavedArticlesByUser failed: " << e.what() << "\n";
    }

    return articles;
}

bool SavedArticleRepository::deleteSavedArticle(int userId, int articleId) {
    if (!db || !db->isConnected()) return false;

    try {
        auto conn = db->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("DELETE FROM saved_articles WHERE user_id = ? AND article_id = ?")
        );

        stmt->setInt(1, userId);
        stmt->setInt(2, articleId);

        stmt->execute();
        return true;

    } catch (const sql::SQLException& e) {
        std::cerr << "[SavedArticleRepository] deleteSavedArticle failed: " << e.what() << "\n";
        return false;
    }
}
