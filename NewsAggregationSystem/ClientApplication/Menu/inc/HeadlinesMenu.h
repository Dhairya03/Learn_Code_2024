#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class HeadlinesMenu {
public:
    HeadlinesMenu(Client& client, Session& session);
    void display();

private:
    Client& client;
    Session& session;

    void viewTodayHeadlines();
    void viewDateRangeHeadlines();
    void fetchAndDisplayArticles(const std::string& start, const std::string& end, const std::string& category);
    std::string selectCategory();
};
