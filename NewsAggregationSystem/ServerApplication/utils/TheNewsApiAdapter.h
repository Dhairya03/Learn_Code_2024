#ifndef THENEWSAPIADAPTER_H
#define THENEWSAPIADAPTER_H

#include "INewsApiAdapter.h"
#include <string>
#include <vector>

class TheNewsApiAdapter : public INewsApiAdapter {
public:
    TheNewsApiAdapter(const std::string& apiKey);
    std::vector<NewsArticle> fetchTopHeadlines() override;
private:
    std::string apiKey;
};

#endif 