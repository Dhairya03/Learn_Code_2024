#ifndef NEWSARTICLE_H
#define NEWSARTICLE_H

#include <string>

class NewsArticle {
public:
    int id = 0; 
    std::string title;
    std::string description;
    std::string url;
    std::string source;
    std::string publishedAt; 
    int categoryId;
    std::string categoryName;
    int likes;
    int dislikes;
};

#endif