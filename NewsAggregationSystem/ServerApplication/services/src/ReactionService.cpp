#include "services/inc/ReactionService.h"
#include "repositories/inc/ArticleReactionRepository.h"
#include <iostream>

ReactionService::ReactionService(std::shared_ptr<DBConnection> dbConn) : db(std::move(dbConn)) {}

bool ReactionService::likeArticle(int userId, int articleId) {
    std::cout << "[ReactionService] likeArticle called for user " << userId << " on article " << articleId << std::endl;
    try {
        ArticleReactionRepository repo(db);
        bool result = repo.addReaction(userId, articleId, ReactionType::LIKE);
        std::cout << "[ReactionService] likeArticle success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[ReactionService] likeArticle error: " << e.what() << std::endl;
        return false;
    }
}

bool ReactionService::dislikeArticle(int userId, int articleId) {
    std::cout << "[ReactionService] dislikeArticle called for user " << userId << " on article " << articleId << std::endl;
    try {
        ArticleReactionRepository repo(db);
        bool result = repo.addReaction(userId, articleId, ReactionType::DISLIKE);
        std::cout << "[ReactionService] dislikeArticle success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[ReactionService] dislikeArticle error: " << e.what() << std::endl;
        return false;
    }
}

bool ReactionService::removeReaction(int userId, int articleId) {
    std::cout << "[ReactionService] removeReaction called for user " << userId << " on article " << articleId << std::endl;
    try {
        ArticleReactionRepository repo(db);
        bool result = repo.removeReaction(userId, articleId);
        std::cout << "[ReactionService] removeReaction success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[ReactionService] removeReaction error: " << e.what() << std::endl;
        return false;
    }
}

ReactionType ReactionService::getUserReaction(int userId, int articleId) {
    try {
        ArticleReactionRepository repo(db);
        return repo.getUserReaction(userId, articleId);
    } catch (const std::exception& e) {
        std::cerr << "[ReactionService] getUserReaction error: " << e.what() << std::endl;
        return ReactionType::NONE;
    }
}

int ReactionService::getLikeCount(int articleId) {
    try {
        ArticleReactionRepository repo(db);
        return repo.getLikeCount(articleId);
    } catch (const std::exception& e) {
        std::cerr << "[ReactionService] getLikeCount error: " << e.what() << std::endl;
        return 0;
    }
}

int ReactionService::getDislikeCount(int articleId) {
    try {
        ArticleReactionRepository repo(db);
        return repo.getDislikeCount(articleId);
    } catch (const std::exception& e) {
        std::cerr << "[ReactionService] getDislikeCount error: " << e.what() << std::endl;
        return 0;
    }
}

std::vector<ArticleReaction> ReactionService::getUserReactions(int userId) {
    try {
        ArticleReactionRepository repo(db);
        return repo.getUserReactions(userId);
    } catch (const std::exception& e) {
        std::cerr << "[ReactionService] getUserReactions error: " << e.what() << std::endl;
        return {};
    }
} 