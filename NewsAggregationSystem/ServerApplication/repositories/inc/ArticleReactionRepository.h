#ifndef ARTICLE_REACTION_REPOSITORY_H
#define ARTICLE_REACTION_REPOSITORY_H

#include "models/inc/ArticleReaction.h"
#include <memory>
#include <vector>
#include "database/inc/DBConnection.h"

class ArticleReactionRepository {
public:
    explicit ArticleReactionRepository(std::shared_ptr<DBConnection> dbConn);
    
    bool addReaction(int userId, int articleId, ReactionType reactionType);
    bool updateReaction(int userId, int articleId, ReactionType reactionType);
    bool removeReaction(int userId, int articleId);
    ReactionType getUserReaction(int userId, int articleId);
    
    int getLikeCount(int articleId);
    int getDislikeCount(int articleId);
    
    std::vector<ArticleReaction> getUserReactions(int userId);

private:
    std::shared_ptr<DBConnection> db;
    void createTablesIfNotExist();
    void updateArticleCounts(int articleId);
};

#endif 