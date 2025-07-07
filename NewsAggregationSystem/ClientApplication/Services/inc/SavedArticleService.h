#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class SavedArticleService {
public:
    SavedArticleService(Client& httpClient, Session& userSession);
    void saveArticle(int articleId);
    void deleteArticle(int articleId);

private:
    Client& httpClient;
    Session& userSession;
};
