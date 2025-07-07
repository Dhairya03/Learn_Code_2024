#ifndef INEWSAPIADAPTER_H
#define INEWSAPIADAPTER_H

#include <vector>
#include <string>
#include "models/inc/NewsArticle.h"

class INewsApiAdapter {
public:
    virtual ~INewsApiAdapter() = default;
    virtual std::vector<NewsArticle> fetchTopHeadlines() = 0;
};

#endif 