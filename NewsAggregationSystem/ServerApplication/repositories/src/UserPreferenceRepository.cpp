#include "repositories/inc/UserPreferenceRepository.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <iostream>
#include <algorithm>
#include <cmath>

UserPreferenceRepository::UserPreferenceRepository(std::shared_ptr<DBConnection> dbConn) 
    : db(std::move(dbConn)) {
    createTablesIfNotExist();
}

void UserPreferenceRepository::createTablesIfNotExist() {
    std::cout << "[UserPreferenceRepository] createTablesIfNotExist called" << std::endl;
    auto conn = db->getConnection();
    try {
        std::string createReadHistoryTable = R"(
            CREATE TABLE IF NOT EXISTS user_read_history (
                id INT AUTO_INCREMENT PRIMARY KEY,
                user_id INT NOT NULL,
                article_id INT NOT NULL,
                read_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                UNIQUE KEY unique_user_article_read (user_id, article_id),
                FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
                FOREIGN KEY (article_id) REFERENCES articles(id) ON DELETE CASCADE
            )
        )";
        
        auto stmt = conn->createStatement();
        stmt->execute(createReadHistoryTable);
        
        std::cout << "[UserPreferenceRepository] createTablesIfNotExist success" << std::endl;
    } catch (const sql::SQLException& e) {
        std::cerr << "[UserPreferenceRepository] Error creating tables: " << e.what() << std::endl;
    }
}

UserPreference UserPreferenceRepository::getUserPreference(int userId) {
    UserPreference preference;
    preference.userId = userId;
    
    try {
        auto conn = db->getConnection();
        
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT * FROM user_notification_settings WHERE user_id = ?"));
        stmt->setInt(1, userId);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        
        if (res->next()) {
            preference.businessEnabled = res->getBoolean("business_enabled");
            preference.entertainmentEnabled = res->getBoolean("entertainment_enabled");
            preference.sportsEnabled = res->getBoolean("sports_enabled");
            preference.technologyEnabled = res->getBoolean("technology_enabled");
            preference.keywordsEnabled = res->getBoolean("keywords_enabled");
            preference.keywords = res->getString("keywords");
        }
        
        preference.likedArticleIds = getUserLikedArticles(userId);
        preference.savedArticleIds = getUserSavedArticles(userId);
        preference.readArticleIds = getUserReadArticles(userId);
        
    } catch (const sql::SQLException& e) {
        std::cerr << "[UserPreferenceRepository] Error getting user preference: " << e.what() << std::endl;
    }
    
    return preference;
}

bool UserPreferenceRepository::updateUserPreference(const UserPreference& preference) {
    try {
        auto conn = db->getConnection();
        
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(
                "UPDATE user_notification_settings SET "
                "business_enabled = ?, entertainment_enabled = ?, sports_enabled = ?, "
                "technology_enabled = ?, keywords_enabled = ?, keywords = ? "
                "WHERE user_id = ?"));
        
        stmt->setBoolean(1, preference.businessEnabled);
        stmt->setBoolean(2, preference.entertainmentEnabled);
        stmt->setBoolean(3, preference.sportsEnabled);
        stmt->setBoolean(4, preference.technologyEnabled);
        stmt->setBoolean(5, preference.keywordsEnabled);
        stmt->setString(6, preference.keywords);
        stmt->setInt(7, preference.userId);
        
        return stmt->executeUpdate() > 0;
    } catch (const sql::SQLException& e) {
        std::cerr << "[UserPreferenceRepository] Error updating user preference: " << e.what() << std::endl;
        return false;
    }
}

bool UserPreferenceRepository::trackArticleRead(int userId, int articleId) {
    try {
        auto conn = db->getConnection();
        
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(
                "INSERT INTO user_read_history (user_id, article_id) VALUES (?, ?) "
                "ON DUPLICATE KEY UPDATE read_at = CURRENT_TIMESTAMP"));
        
        stmt->setInt(1, userId);
        stmt->setInt(2, articleId);
        
        return stmt->executeUpdate() > 0;
    } catch (const sql::SQLException& e) {
        std::cerr << "[UserPreferenceRepository] Error tracking article read: " << e.what() << std::endl;
        return false;
    }
}

std::vector<int> UserPreferenceRepository::getUserReadArticles(int userId) {
    std::vector<int> articleIds;
    try {
        auto conn = db->getConnection();
        
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT article_id FROM user_read_history WHERE user_id = ? ORDER BY read_at DESC"));
        stmt->setInt(1, userId);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        
        while (res->next()) {
            articleIds.push_back(res->getInt("article_id"));
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "[UserPreferenceRepository] Error getting user read articles: " << e.what() << std::endl;
    }
    return articleIds;
}

std::vector<int> UserPreferenceRepository::getUserLikedArticles(int userId) {
    std::vector<int> articleIds;
    try {
        auto conn = db->getConnection();
        
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT article_id FROM article_reactions WHERE user_id = ? AND reaction_type = 'LIKE'"));
        stmt->setInt(1, userId);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        
        while (res->next()) {
            articleIds.push_back(res->getInt("article_id"));
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "[UserPreferenceRepository] Error getting user liked articles: " << e.what() << std::endl;
    }
    return articleIds;
}

std::vector<int> UserPreferenceRepository::getUserSavedArticles(int userId) {
    std::vector<int> articleIds;
    try {
        auto conn = db->getConnection();
        
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT article_id FROM saved_articles WHERE user_id = ?"));
        stmt->setInt(1, userId);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        
        while (res->next()) {
            articleIds.push_back(res->getInt("article_id"));
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "[UserPreferenceRepository] Error getting user saved articles: " << e.what() << std::endl;
    }
    return articleIds;
}

std::vector<NewsArticle> UserPreferenceRepository::getPersonalizedArticles(int userId, int limit) {
    UserPreference preference = getUserPreference(userId);
    std::vector<NewsArticle> articles;
    
    try {
        auto conn = db->getConnection();
        
        std::string query = R"(
            SELECT DISTINCT a.id, a.title, a.description, a.url, a.source, a.published_at, 
                   a.category_id, c.name as category_name, a.likes, a.dislikes
            FROM articles a
            LEFT JOIN categories c ON a.category_id = c.id
            WHERE 1=1
        )";
        
        std::vector<std::string> conditions;
        std::vector<std::string> params;
        
        auto enabledCategories = preference.getEnabledCategories();
        if (!enabledCategories.empty()) {
            std::string categoryCondition = "c.name IN (";
            for (size_t i = 0; i < enabledCategories.size(); ++i) {
                if (i > 0) categoryCondition += ",";
                categoryCondition += "?";
                params.push_back(enabledCategories[i]);
            }
            categoryCondition += ")";
            conditions.push_back(categoryCondition);
        }
        
        if (preference.keywordsEnabled && !preference.keywords.empty()) {
            auto keywords = preference.getKeywordsList();
            for (const auto& keyword : keywords) {
                conditions.push_back("(a.title LIKE ? OR a.description LIKE ?)");
                params.push_back("%" + keyword + "%");
                params.push_back("%" + keyword + "%");
            }
        }
        
        if (!preference.likedArticleIds.empty() || !preference.savedArticleIds.empty()) {
            std::vector<int> behaviorArticles;
            behaviorArticles.insert(behaviorArticles.end(), preference.likedArticleIds.begin(), preference.likedArticleIds.end());
            behaviorArticles.insert(behaviorArticles.end(), preference.savedArticleIds.begin(), preference.savedArticleIds.end());
            
            if (!behaviorArticles.empty()) {
                std::string similarCondition = "a.category_id IN (SELECT DISTINCT category_id FROM articles WHERE id IN (";
                for (size_t i = 0; i < behaviorArticles.size(); ++i) {
                    if (i > 0) similarCondition += ",";
                    similarCondition += "?";
                    params.push_back(std::to_string(behaviorArticles[i]));
                }
                similarCondition += "))";
                conditions.push_back(similarCondition);
            }
        }
        
        if (!conditions.empty()) {
            query += " AND (" + conditions[0];
            for (size_t i = 1; i < conditions.size(); ++i) {
                query += " OR " + conditions[i];
            }
            query += ")";
        }
        
        query += " ORDER BY a.published_at DESC LIMIT ?";
        params.push_back(std::to_string(limit));
        
        std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(query));
        for (size_t i = 0; i < params.size(); ++i) {
            stmt->setString(i + 1, params[i]);
        }
        
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        while (res->next()) {
            NewsArticle article;
            article.id = res->getInt("id");
            article.title = res->getString("title");
            article.description = res->getString("description");
            article.url = res->getString("url");
            article.source = res->getString("source");
            article.publishedAt = res->getString("published_at");
            article.categoryId = res->getInt("category_id");
            article.categoryName = res->getString("category_name");
            article.likes = res->getInt("likes");
            article.dislikes = res->getInt("dislikes");
            articles.push_back(article);
        }
        
        articles = rankArticlesByPreference(articles, preference);
        
    } catch (const sql::SQLException& e) {
        std::cerr << "[UserPreferenceRepository] Error getting personalized articles: " << e.what() << std::endl;
    }
    
    return articles;
}

std::vector<NewsArticle> UserPreferenceRepository::getRecommendedArticles(int userId, int limit) {
    UserPreference preference = getUserPreference(userId);
    std::vector<NewsArticle> articles = getPersonalizedArticles(userId, limit * 2); 
    
    std::sort(articles.begin(), articles.end(), 
        [&](const NewsArticle& a, const NewsArticle& b) {
            return calculateArticleScore(a, preference) > calculateArticleScore(b, preference);
        });
    
    if (articles.size() > static_cast<size_t>(limit)) {
        articles.resize(limit);
    }
    
    return articles;
}

double UserPreferenceRepository::calculateArticleScore(const NewsArticle& article, const UserPreference& preference) {
    double score = 0.0;
    
    if (preference.hasCategoryPreference(article.categoryName)) {
        score += preference.categoryWeight;
    }
    
    if (preference.hasKeywordMatch(article.title + " " + article.description)) {
        score += preference.keywordWeight;
    }
    
    std::vector<int> userBehaviorArticles;
    userBehaviorArticles.insert(userBehaviorArticles.end(), preference.likedArticleIds.begin(), preference.likedArticleIds.end());
    userBehaviorArticles.insert(userBehaviorArticles.end(), preference.savedArticleIds.begin(), preference.savedArticleIds.end());
    
    for (int behaviorArticleId : userBehaviorArticles) {
        if (std::find(userBehaviorArticles.begin(), userBehaviorArticles.end(), behaviorArticleId) != userBehaviorArticles.end()) {
            score += preference.likeWeight * 0.5; 
        }
    }
    
    double popularityScore = (article.likes - article.dislikes) * 0.1;
    score += popularityScore;
    score += 0.1;
    
    return score;
}

std::vector<NewsArticle> UserPreferenceRepository::rankArticlesByPreference(const std::vector<NewsArticle>& articles, const UserPreference& preference) {
    std::vector<std::pair<NewsArticle, double>> scoredArticles;
    
    for (const auto& article : articles) {
        double score = calculateArticleScore(article, preference);
        scoredArticles.push_back({article, score});
    }
    
    std::sort(scoredArticles.begin(), scoredArticles.end(),
        [](const std::pair<NewsArticle, double>& a, const std::pair<NewsArticle, double>& b) {
            return a.second > b.second;
        });
    
    std::vector<NewsArticle> rankedArticles;
    for (const auto& scoredArticle : scoredArticles) {
        rankedArticles.push_back(scoredArticle.first);
    }
    
    return rankedArticles;
} 