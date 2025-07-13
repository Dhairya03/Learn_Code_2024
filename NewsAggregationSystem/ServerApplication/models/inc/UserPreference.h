#ifndef USER_PREFERENCE_H
#define USER_PREFERENCE_H

#include <string>
#include <vector>

class UserPreference {
public:
    int userId;
    
    bool businessEnabled = false;
    bool entertainmentEnabled = false;
    bool sportsEnabled = false;
    bool technologyEnabled = false;
    bool keywordsEnabled = false;
    std::string keywords;
    
    std::vector<int> likedArticleIds;
    std::vector<int> savedArticleIds;
    std::vector<int> readArticleIds;
    
    double categoryWeight = 1.0;
    double keywordWeight = 1.0;
    double likeWeight = 1.0;
    double saveWeight = 1.0;
    double readWeight = 0.5;
    
    std::vector<std::string> getEnabledCategories() const;
    std::vector<std::string> getKeywordsList() const;
    bool hasCategoryPreference(const std::string& category) const;
    bool hasKeywordMatch(const std::string& text) const;
};

#endif 