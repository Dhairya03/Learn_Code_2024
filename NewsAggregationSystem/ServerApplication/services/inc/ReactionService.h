#ifndef REACTION_SERVICE_H
#define REACTION_SERVICE_H

#include "models/inc/ArticleReaction.h"
#include <memory>
#include <vector>
#include "database/inc/DBConnection.h"

class ReactionService {
public:
    explicit ReactionService(std::shared_ptr<DBConnection> dbConn);
    
    // Core reaction methods
    bool likeArticle(int userId, int articleId);
    bool dislikeArticle(int userId, int articleId);
    bool removeReaction(int userId, int articleId);
    ReactionType getUserReaction(int userId, int articleId);
    
    // Get reaction counts
    int getLikeCount(int articleId);
    int getDislikeCount(int articleId);
    
    // Get user's reaction history
    std::vector<ArticleReaction> getUserReactions(int userId);

private:
    std::shared_ptr<DBConnection> db;
};

#endif 