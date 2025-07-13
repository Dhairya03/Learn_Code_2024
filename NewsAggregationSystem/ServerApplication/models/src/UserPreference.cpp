#include "../inc/UserPreference.h"
#include <algorithm>
#include <sstream>
#include <cctype>

std::vector<std::string> UserPreference::getEnabledCategories() const {
    std::vector<std::string> categories;
    if (businessEnabled) categories.push_back("business");
    if (entertainmentEnabled) categories.push_back("entertainment");
    if (sportsEnabled) categories.push_back("sports");
    if (technologyEnabled) categories.push_back("technology");
    return categories;
}

std::vector<std::string> UserPreference::getKeywordsList() const {
    std::vector<std::string> keywordList;
    if (keywords.empty()) return keywordList;
    
    std::istringstream iss(keywords);
    std::string keyword;
    while (std::getline(iss, keyword, ',')) {
        keyword.erase(0, keyword.find_first_not_of(" \t"));
        keyword.erase(keyword.find_last_not_of(" \t") + 1);
        if (!keyword.empty()) {
            keywordList.push_back(keyword);
        }
    }
    return keywordList;
}

bool UserPreference::hasCategoryPreference(const std::string& category) const {
    std::string lowerCategory = category;
    std::transform(lowerCategory.begin(), lowerCategory.end(), lowerCategory.begin(), ::tolower);
    
    if (lowerCategory == "business" && businessEnabled) return true;
    if (lowerCategory == "entertainment" && entertainmentEnabled) return true;
    if (lowerCategory == "sports" && sportsEnabled) return true;
    if (lowerCategory == "technology" && technologyEnabled) return true;
    
    return false;
}

bool UserPreference::hasKeywordMatch(const std::string& text) const {
    if (!keywordsEnabled || keywords.empty()) return false;
    
    std::string lowerText = text;
    std::transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::tolower);
    
    auto keywordList = getKeywordsList();
    for (const auto& keyword : keywordList) {
        std::string lowerKeyword = keyword;
        std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
        
        if (lowerText.find(lowerKeyword) != std::string::npos) {
            return true;
        }
    }
    
    return false;
} 