#ifndef NEWSAPIORGADAPTER_H
#define NEWSAPIORGADAPTER_H

#include "INewsApiAdapter.h"
#include <string>
#include <vector>

class NewsApiOrgAdapter : public INewsApiAdapter {
public:
    NewsApiOrgAdapter(const std::string& apiKey);
    std::vector<NewsArticle> fetchTopHeadlines() override;
private:
    std::string apiKey;
};

#endif 