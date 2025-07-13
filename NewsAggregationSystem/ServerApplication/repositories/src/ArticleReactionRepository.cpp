#include "repositories/inc/ArticleReactionRepository.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <iostream>

ArticleReactionRepository::ArticleReactionRepository(std::shared_ptr<DBConnection> dbConn) 
    : db(std::move(dbConn)) {
    createTablesIfNotExist();
}

void ArticleReactionRepository::createTablesIfNotExist() {
    std::cout << "[ArticleReactionRepository] createTablesIfNotExist called" << std::endl;
    auto conn = db->getConnection();
    try {
        // Create article_reactions table
        std::string createReactionsTable = R"(
            CREATE TABLE IF NOT EXISTS article_reactions (
                id INT AUTO_INCREMENT PRIMARY KEY,
                user_id INT NOT NULL,
                article_id INT NOT NULL,
                reaction_type ENUM('LIKE', 'DISLIKE') NOT NULL,
                timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                UNIQUE KEY unique_user_article (user_id, article_id),
                FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
                FOREIGN KEY (article_id) REFERENCES articles(id) ON DELETE CASCADE
            )
        )";
        
        auto stmt = conn->createStatement();
        stmt->execute(createReactionsTable);
        
        // Add likes and dislikes columns to articles table if they don't exist
        std::string addLikesColumn = R"(
            ALTER TABLE articles ADD COLUMN IF NOT EXISTS likes INT DEFAULT 0
        )";
        try {
            auto stmt2 = conn->createStatement();
            stmt2->execute(addLikesColumn);
        } catch (const sql::SQLException& e) {
            std::cerr << "[ArticleReactionRepository] Error adding likes column: " << e.what() << std::endl;
        }
        
        std::string addDislikesColumn = R"(
            ALTER TABLE articles ADD COLUMN IF NOT EXISTS dislikes INT DEFAULT 0
        )";
        try {
            auto stmt3 = conn->createStatement();
            stmt3->execute(addDislikesColumn);
        } catch (const sql::SQLException& e) {
            std::cerr << "[ArticleReactionRepository] Error adding dislikes column: " << e.what() << std::endl;
        }
        
        std::cout << "[ArticleReactionRepository] createTablesIfNotExist success" << std::endl;
    } catch (const sql::SQLException& e) {
        std::cerr << "[ArticleReactionRepository] Error creating tables: " << e.what() << std::endl;
    }
}

bool ArticleReactionRepository::addReaction(int userId, int articleId, ReactionType reactionType) {
    if (!db || !db->isConnected()) {
        std::cerr << "[ArticleReactionRepository] Database not connected" << std::endl;
        return false;
    }
    
    try {
        auto conn = db->getConnection();
        
        // Check if user already has a reaction for this article
        std::unique_ptr<sql::PreparedStatement> checkStmt(
            conn->prepareStatement("SELECT reaction_type FROM article_reactions WHERE user_id = ? AND article_id = ?"));
        checkStmt->setInt(1, userId);
        checkStmt->setInt(2, articleId);
        std::unique_ptr<sql::ResultSet> checkRes(checkStmt->executeQuery());
        
        if (checkRes->next()) {
            // User already has a reaction, update it
            return updateReaction(userId, articleId, reactionType);
        } else {
            // Add new reaction
            std::unique_ptr<sql::PreparedStatement> stmt(
                conn->prepareStatement("INSERT INTO article_reactions (user_id, article_id, reaction_type) VALUES (?, ?, ?)"));
            stmt->setInt(1, userId);
            stmt->setInt(2, articleId);
            stmt->setString(3, reactionType == ReactionType::LIKE ? "LIKE" : "DISLIKE");
            stmt->execute();
            
            // Update article counts
            updateArticleCounts(articleId);
            
            std::cout << "[ArticleReactionRepository] Added reaction for user " << userId << " on article " << articleId << std::endl;
            return true;
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "[ArticleReactionRepository] Error adding reaction: " << e.what() << std::endl;
        return false;
    }
}

bool ArticleReactionRepository::updateReaction(int userId, int articleId, ReactionType reactionType) {
    if (!db || !db->isConnected()) {
        std::cerr << "[ArticleReactionRepository] Database not connected" << std::endl;
        return false;
    }
    
    try {
        auto conn = db->getConnection();
        
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("UPDATE article_reactions SET reaction_type = ? WHERE user_id = ? AND article_id = ?"));
        stmt->setString(1, reactionType == ReactionType::LIKE ? "LIKE" : "DISLIKE");
        stmt->setInt(2, userId);
        stmt->setInt(3, articleId);
        stmt->execute();
        
        // Update article counts
        updateArticleCounts(articleId);
        
        std::cout << "[ArticleReactionRepository] Updated reaction for user " << userId << " on article " << articleId << std::endl;
        return true;
    } catch (const sql::SQLException& e) {
        std::cerr << "[ArticleReactionRepository] Error updating reaction: " << e.what() << std::endl;
        return false;
    }
}

bool ArticleReactionRepository::removeReaction(int userId, int articleId) {
    if (!db || !db->isConnected()) {
        std::cerr << "[ArticleReactionRepository] Database not connected" << std::endl;
        return false;
    }
    
    try {
        auto conn = db->getConnection();
        
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("DELETE FROM article_reactions WHERE user_id = ? AND article_id = ?"));
        stmt->setInt(1, userId);
        stmt->setInt(2, articleId);
        stmt->execute();
        
        // Update article counts
        updateArticleCounts(articleId);
        
        std::cout << "[ArticleReactionRepository] Removed reaction for user " << userId << " on article " << articleId << std::endl;
        return true;
    } catch (const sql::SQLException& e) {
        std::cerr << "[ArticleReactionRepository] Error removing reaction: " << e.what() << std::endl;
        return false;
    }
}

ReactionType ArticleReactionRepository::getUserReaction(int userId, int articleId) {
    if (!db || !db->isConnected()) {
        return ReactionType::NONE;
    }
    
    try {
        auto conn = db->getConnection();
        
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT reaction_type FROM article_reactions WHERE user_id = ? AND article_id = ?"));
        stmt->setInt(1, userId);
        stmt->setInt(2, articleId);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        
        if (res->next()) {
            std::string reactionType = res->getString("reaction_type");
            return reactionType == "LIKE" ? ReactionType::LIKE : ReactionType::DISLIKE;
        }
        
        return ReactionType::NONE;
    } catch (const sql::SQLException& e) {
        std::cerr << "[ArticleReactionRepository] Error getting user reaction: " << e.what() << std::endl;
        return ReactionType::NONE;
    }
}

int ArticleReactionRepository::getLikeCount(int articleId) {
    if (!db || !db->isConnected()) {
        return 0;
    }
    
    try {
        auto conn = db->getConnection();
        
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT COUNT(*) as count FROM article_reactions WHERE article_id = ? AND reaction_type = 'LIKE'"));
        stmt->setInt(1, articleId);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        
        if (res->next()) {
            return res->getInt("count");
        }
        
        return 0;
    } catch (const sql::SQLException& e) {
        std::cerr << "[ArticleReactionRepository] Error getting like count: " << e.what() << std::endl;
        return 0;
    }
}

int ArticleReactionRepository::getDislikeCount(int articleId) {
    if (!db || !db->isConnected()) {
        return 0;
    }
    
    try {
        auto conn = db->getConnection();
        
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT COUNT(*) as count FROM article_reactions WHERE article_id = ? AND reaction_type = 'DISLIKE'"));
        stmt->setInt(1, articleId);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        
        if (res->next()) {
            return res->getInt("count");
        }
        
        return 0;
    } catch (const sql::SQLException& e) {
        std::cerr << "[ArticleReactionRepository] Error getting dislike count: " << e.what() << std::endl;
        return 0;
    }
}

std::vector<ArticleReaction> ArticleReactionRepository::getUserReactions(int userId) {
    std::vector<ArticleReaction> reactions;
    if (!db || !db->isConnected()) {
        return reactions;
    }
    
    try {
        auto conn = db->getConnection();
        
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT * FROM article_reactions WHERE user_id = ? ORDER BY timestamp DESC"));
        stmt->setInt(1, userId);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        
        while (res->next()) {
            ArticleReaction reaction;
            reaction.id = res->getInt("id");
            reaction.userId = res->getInt("user_id");
            reaction.articleId = res->getInt("article_id");
            reaction.timestamp = res->getString("timestamp");
            
            std::string reactionType = res->getString("reaction_type");
            reaction.reactionType = reactionType == "LIKE" ? ReactionType::LIKE : ReactionType::DISLIKE;
            
            reactions.push_back(reaction);
        }
        
        return reactions;
    } catch (const sql::SQLException& e) {
        std::cerr << "[ArticleReactionRepository] Error getting user reactions: " << e.what() << std::endl;
        return reactions;
    }
}

void ArticleReactionRepository::updateArticleCounts(int articleId) {
    try {
        auto conn = db->getConnection();
        
        // Update likes count
        std::unique_ptr<sql::PreparedStatement> likeStmt(
            conn->prepareStatement("UPDATE articles SET likes = (SELECT COUNT(*) FROM article_reactions WHERE article_id = ? AND reaction_type = 'LIKE') WHERE id = ?"));
        likeStmt->setInt(1, articleId);
        likeStmt->setInt(2, articleId);
        likeStmt->execute();
        
        // Update dislikes count
        std::unique_ptr<sql::PreparedStatement> dislikeStmt(
            conn->prepareStatement("UPDATE articles SET dislikes = (SELECT COUNT(*) FROM article_reactions WHERE article_id = ? AND reaction_type = 'DISLIKE') WHERE id = ?"));
        dislikeStmt->setInt(1, articleId);
        dislikeStmt->setInt(2, articleId);
        dislikeStmt->execute();
        
    } catch (const sql::SQLException& e) {
        std::cerr << "[ArticleReactionRepository] Error updating article counts: " << e.what() << std::endl;
    }
} 