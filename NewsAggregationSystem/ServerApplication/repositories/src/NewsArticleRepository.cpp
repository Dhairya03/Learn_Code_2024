#include "repositories/inc/NewsArticleRepository.h"
#include "database/inc/DBConnection.h"
#include <cppconn/prepared_statement.h>
#include <iostream>
#include <regex>

NewsArticleRepository::NewsArticleRepository(std::shared_ptr<DBConnection> dbConn) : db(std::move(dbConn)) {}

std::vector<NewsArticle> NewsArticleRepository::getAllArticles()
{
    std::vector<NewsArticle> articles;

    if (!db || !db->isConnected())
        return articles;

    try
    {
        auto conn = db->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT id, title, description, url, source, published_at FROM articles ORDER BY published_at DESC LIMIT 50"));

        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        while (res->next())
        {
            NewsArticle article;
            article.id = res->getInt("id");
            article.title = res->getString("title");
            article.description = res->getString("description");
            article.url = res->getString("url");
            article.source = res->getString("source");
            article.publishedAt = res->getString("published_at");

            articles.push_back(article);
        }
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "[NewsArticleRepository] Error: " << e.what() << "\n";
    }

    return articles;
}

std::vector<NewsArticle> NewsArticleRepository::getArticlesByCategory(const std::string &categoryName)
{
    std::vector<NewsArticle> articles;

    if (!db || !db->isConnected())
        return articles;

    try
    {
        auto conn = db->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(
                "SELECT a.id, a.title, a.description, a.url, a.source, a.published_at "
                "FROM articles a "
                "JOIN categories c ON a.category_id = c.id "
                "WHERE c.name = ? "
                "ORDER BY a.published_at DESC"));

        stmt->setString(1, categoryName);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        while (res->next())
        {
            NewsArticle article;
            article.id = res->getInt("id");
            article.title = res->getString("title");
            article.description = res->getString("description");
            article.url = res->getString("url");
            article.source = res->getString("source");
            article.publishedAt = res->getString("published_at");

            articles.push_back(article);
        }
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "[NewsArticleRepository] Category fetch error: " << e.what() << "\n";
    }

    return articles;
}

bool isValidDateTime(const std::string &dt)
{
    if (dt.empty())
        return false;

    std::regex iso8601_regex(R"(\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}(?:\.\d{3})?Z?)");
    if (std::regex_match(dt, iso8601_regex))
    {
        return true;
    }

    std::regex mysql_regex(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})");
    return std::regex_match(dt, mysql_regex);
}

std::string convertToMySQLDateTime(const std::string &isoDateTime)
{
    if (isoDateTime.empty())
    {
        return "";
    }

    if (isoDateTime.find('T') == std::string::npos)
    {
        return isoDateTime;
    }

    std::string result = isoDateTime;

    size_t tPos = result.find('T');
    if (tPos != std::string::npos)
    {
        result[tPos] = ' ';
    }

    size_t zPos = result.find('Z');
    if (zPos != std::string::npos)
    {
        result = result.substr(0, zPos);
    }

    size_t dotPos = result.find('.');
    if (dotPos != std::string::npos)
    {
        result = result.substr(0, dotPos);
    }

    return result;
}

bool NewsArticleRepository::saveArticle(const NewsArticle &article)
{
    if (!db || !db->isConnected())
    {
        std::cerr << "[NewsArticleRepository] Database not connected\n";
        return false;
    }

    if (article.title.empty() || article.url.empty())
    {
        std::cerr << "[NewsArticleRepository] Article missing essential fields (title or URL)\n";
        return false;
    }

    try
    {
        auto conn = db->getConnection();

        std::unique_ptr<sql::PreparedStatement> checkStmt(
            conn->prepareStatement("SELECT COUNT(*) as count FROM articles WHERE url = ?"));
        checkStmt->setString(1, article.url);
        std::unique_ptr<sql::ResultSet> checkRes(checkStmt->executeQuery());

        if (checkRes->next() && checkRes->getInt("count") > 0)
        {
            std::cout << "[NewsArticleRepository] Article already exists, skipping: " << article.title << "\n";
            return true;
        }

        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(
                "INSERT INTO articles (title, description, url, source, category_id, published_at) "
                "VALUES (?, ?, ?, ?, ?, ?)"));

        stmt->setString(1, article.title);
        stmt->setString(2, article.description.empty() ? "No description available" : article.description);
        stmt->setString(3, article.url);
        stmt->setString(4, article.source.empty() ? "Unknown Source" : article.source);

        int categoryId = article.categoryId;
        if (categoryId <= 0)
        {
            std::cout << "[NewsArticleRepository] Invalid category ID, defaulting to general (7)\n";
            categoryId = 7;
        }
        stmt->setInt(5, categoryId);

        if (article.publishedAt.empty())
        {
            stmt->setNull(6, sql::DataType::TIMESTAMP);
        }
        else
        {
            std::string mysqlDateTime = convertToMySQLDateTime(article.publishedAt);
            if (isValidDateTime(mysqlDateTime))
            {
                stmt->setString(6, mysqlDateTime);
            }
            else
            {
                std::cout << "[NewsArticleRepository] Invalid datetime format, setting to NULL: " << article.publishedAt << "\n";
                stmt->setNull(6, sql::DataType::TIMESTAMP);
            }
        }

        stmt->execute();
        std::cout << "[NewsArticleRepository] Successfully saved article: " << article.title << "\n";
        return true;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "[NewsArticleRepository] Failed to save article '" << article.title << "': " << e.what() << std::endl;
        return false;
    }
}

std::vector<NewsArticle> NewsArticleRepository::searchArticles(const std::string& query, const std::string& startDate, const std::string& endDate, const std::string& sort) {
    std::vector<NewsArticle> articles;
    auto conn = db->getConnection();
    std::string sql = "SELECT a.id, a.title, a.description, a.url, a.source, a.published_at, a.category_id, c.name as category_name, a.likes, a.dislikes FROM articles a LEFT JOIN categories c ON a.category_id = c.id WHERE 1=1";
    if (!query.empty()) {
        sql += " AND (a.title LIKE ? OR a.description LIKE ?)";
    }
    if (!startDate.empty()) {
        sql += " AND a.published_at >= ?";
    }
    if (!endDate.empty()) {
        sql += " AND a.published_at <= ?";
    }
    if (sort == "likes") {
        sql += " ORDER BY a.likes DESC";
    } else if (sort == "dislikes") {
        sql += " ORDER BY a.dislikes DESC";
    } else {
        sql += " ORDER BY a.published_at DESC";
    }
    try {
        std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(sql));
        int idx = 1;
        if (!query.empty()) {
            std::string likeQuery = "%" + query + "%";
            stmt->setString(idx++, likeQuery);
            stmt->setString(idx++, likeQuery);
        }
        if (!startDate.empty()) {
            stmt->setString(idx++, startDate);
        }
        if (!endDate.empty()) {
            stmt->setString(idx++, endDate);
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
    } catch (const sql::SQLException& e) {
        std::cerr << "[NewsArticleRepository] searchArticles error: " << e.what() << std::endl;
    }
    return articles;
}

std::vector<NewsArticle> NewsArticleRepository::getArticlesToday()
{
    std::vector<NewsArticle> articles;
    if (!db || !db->isConnected())
        return articles;

    auto conn = db->getConnection();
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement(
            "SELECT a.*, c.name as category_name FROM articles a "
            "JOIN categories c ON a.category_id = c.id "
            "WHERE DATE(published_at) = CURDATE()"));
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

    while (res->next())
    {
        NewsArticle a;
        a.id = res->getInt("id");
        a.title = res->getString("title");
        a.description = res->getString("description");
        a.url = res->getString("url");
        a.source = res->getString("source");
        a.publishedAt = res->getString("published_at");
        a.categoryId = res->getInt("category_id");
        a.categoryName = res->getString("category_name");
        articles.push_back(a);
    }

    return articles;
}

std::vector<NewsArticle> NewsArticleRepository::getArticlesByDateAndCategory(const std::string &startDate, const std::string &endDate, const std::string &categoryName)
{
    std::vector<NewsArticle> articles;

    if (!db || !db->isConnected())
        return articles;

    try
    {
        auto conn = db->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement(
                "SELECT a.id, a.title, a.description, a.url, a.source, a.published_at "
                "FROM articles a "
                "JOIN categories c ON a.category_id = c.id "
                "WHERE c.name = ? "
                "AND a.published_at >= ? "
                "AND a.published_at <= ? "
                "ORDER BY a.published_at DESC"));

        stmt->setString(1, categoryName);
        stmt->setString(2, startDate);
        stmt->setString(3, endDate);

        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        while (res->next())
        {
            NewsArticle article;
            article.id = res->getInt("id");
            article.title = res->getString("title");
            article.description = res->getString("description");
            article.url = res->getString("url");
            article.source = res->getString("source");
            article.publishedAt = res->getString("published_at");

            articles.push_back(article);
        }
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "[NewsArticleRepository] Date and category fetch error: " << e.what() << "\n";
    }

    return articles;
}