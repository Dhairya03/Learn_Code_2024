#pragma once
#include "../../Core/inc/Client.h"
#include "../../Core/inc/Session.h"

class HeadlinesMenu {
public:
    HeadlinesMenu(Client& httpClient, Session& userSession);
    void display();

private:
    Client& httpClient;
    Session& userSession;

    void displayTodayHeadlines();
    void displayDateRangeHeadlines();
    void fetchAndDisplayArticlesTable(const std::string& start, const std::string& end, const std::string& category);
    std::string selectHeadlineCategory();
};
