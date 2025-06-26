#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class SavedArticleService {
public:
    SavedArticleService(Client& client, Session& session);
    void saveArticle(int articleId);
    void deleteArticle(int articleId);

private:
    Client& client;
    Session& session;
};
