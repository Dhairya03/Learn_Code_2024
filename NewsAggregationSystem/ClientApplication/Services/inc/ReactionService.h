#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"
#include <string>
#include <nlohmann/json.hpp>

class ReactionService {
public:
    explicit ReactionService(Client& client, Session& session);
    
    bool likeArticle(int articleId);
    bool dislikeArticle(int articleId);
    bool removeReaction(int articleId);
    std::string getUserReaction(int articleId);
    
    int getLikeCount(int articleId);
    int getDislikeCount(int articleId);

private:
    Client& client;
    Session& session;
}; 