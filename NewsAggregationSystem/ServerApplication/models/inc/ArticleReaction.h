#ifndef ARTICLE_REACTION_H
#define ARTICLE_REACTION_H

#include <string>

enum class ReactionType {
    LIKE,
    DISLIKE,
    NONE
};

class ArticleReaction {
public:
    int id = 0;
    int userId;
    int articleId;
    ReactionType reactionType;
    std::string timestamp;
};

#endif 