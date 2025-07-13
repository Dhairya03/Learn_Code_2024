#include "controllers/inc/NewsController.h"
#include "services/inc/NewsService.h"
#include "lib/json/json.hpp"
#include <iostream>
#include <string>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include "repositories/inc/NotificationRepository.h"
#include "repositories/inc/UserRepository.h"
#include "models/inc/Notification.h"

using json = nlohmann::json;

static nlohmann::json serializeArticle(const NewsArticle& article) {
    nlohmann::json articleJson = {
        {"id", article.id},
        {"title", article.title},
        {"description", article.description},
        {"url", article.url},
        {"source", article.source},
        {"published_at", article.publishedAt},
        {"category", article.categoryName},
        {"categoryId", article.categoryId},
        {"likes", article.likes},
        {"dislikes", article.dislikes}
    };
    return articleJson;
}

crow::response NewsController::getAll(const crow::request&, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[NewsController] getAll called" << std::endl;
    NewsService newsService(dbConn);
    auto articles = newsService.getAllArticles();
    nlohmann::json result = nlohmann::json::array();
    for (const auto& article : articles) {
        result.push_back(serializeArticle(article));
    }
    std::cout << "[NewsController] getAll returning " << result.size() << " articles" << std::endl;
    return crow::response(200, result.dump());
}

crow::response NewsController::getByCategory(const crow::request&, std::shared_ptr<DBConnection> dbConn, const std::string& categoryName) {
    std::cout << "[NewsController] getByCategory called for category: " << categoryName << std::endl;
    NewsService newsService(dbConn);
    auto articles = newsService.getArticlesByCategory(categoryName);
    if (articles.empty()) {
        std::cout << "[NewsController] No articles found in category: " << categoryName << std::endl;
        return crow::response(404, "No articles found in this category");
    }
    nlohmann::json result = nlohmann::json::array();
    for (const auto& article : articles) {
        result.push_back(serializeArticle(article));
    }
    std::cout << "[NewsController] getByCategory returning " << result.size() << " articles" << std::endl;
    return crow::response(200, result.dump());
}

crow::response NewsController::search(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[NewsController] search called" << std::endl;
    auto params = crow::query_string(req.url_params);
    const char* query = params.get("q");
    const char* startDate = params.get("start");
    const char* endDate = params.get("end");
    const char* sort = params.get("sort");
    if (!query) {
        std::cout << "[NewsController] search missing query param" << std::endl;
        return crow::response(400, "Missing search query");
    }
    NewsService newsService(dbConn);
    auto articles = newsService.searchArticles(
        std::string(query),
        startDate ? std::string(startDate) : "",
        endDate ? std::string(endDate) : "",
        sort ? std::string(sort) : ""
    );
    nlohmann::json result = nlohmann::json::array();
    for (const auto& article : articles) {
        result.push_back(serializeArticle(article));
    }
    std::cout << "[NewsController] search returning " << result.size() << " articles" << std::endl;
    return crow::response(200, result.dump());
}

crow::response NewsController::getTodayNews(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[NewsController] getTodayNews called" << std::endl;
    NewsService newsService(dbConn);
    auto articles = newsService.getArticlesToday();
    nlohmann::json result = nlohmann::json::array();
    for (const auto& article : articles) {
        result.push_back(serializeArticle(article));
    }
    std::cout << "[NewsController] getTodayNews returning " << result.size() << " articles" << std::endl;
    return crow::response(200, result.dump());
}

crow::response NewsController::getNewsByDateAndCategory(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[NewsController] getNewsByDateAndCategory called" << std::endl;
    std::string startDate = req.url_params.get("start") ? req.url_params.get("start") : "";
    std::string endDate = req.url_params.get("end") ? req.url_params.get("end") : "";
    std::string category = req.url_params.get("category") ? req.url_params.get("category") : "";
    if (startDate.empty() || endDate.empty()) {
        std::cout << "[NewsController] getNewsByDateAndCategory missing start or end date" << std::endl;
        return crow::response(400, "Missing start or end date");
    }
    NewsService newsService(dbConn);
    auto articles = newsService.getNewsByDateAndCategory(startDate, endDate, category);
    nlohmann::json result = nlohmann::json::array();
    for (const auto& article : articles) {
        result.push_back(serializeArticle(article));
    }
    std::cout << "[NewsController] getNewsByDateAndCategory returning " << result.size() << " articles" << std::endl;
    return crow::response(200, result.dump());
}

crow::response NewsController::searchNews(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[NewsController] searchNews called" << std::endl;
    try {
        std::string query = req.url_params.get("q") ? req.url_params.get("q") : "";
        std::string startDate = req.url_params.get("start_date") ? req.url_params.get("start_date") : "";
        std::string endDate = req.url_params.get("end_date") ? req.url_params.get("end_date") : "";
        std::string sort = req.url_params.get("sort") ? req.url_params.get("sort") : "";
        NewsService newsService(dbConn);
        auto articles = newsService.searchArticles(query, startDate, endDate, sort);
        nlohmann::json result = nlohmann::json::array();
        for (const auto& article : articles) {
            result.push_back(serializeArticle(article));
        }
        std::cout << "[NewsController] searchNews returning " << result.size() << " articles" << std::endl;
        return crow::response(200, result.dump());
    } catch (const std::exception& e) {
        std::cerr << "[NewsController] Error in searchNews: " << e.what() << std::endl;
        return crow::response(500, "Internal server error");
    }
}

crow::response NewsController::reportArticle(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[NewsController] reportArticle called" << std::endl;
    try {
        auto requestBody = nlohmann::json::parse(req.body);
        if (!requestBody.contains("user_id") || !requestBody.contains("article_id")) {
            std::cout << "[NewsController] reportArticle missing user_id or article_id" << std::endl;
            return crow::response(400, "Missing user_id or article_id");
        }
        int userId = requestBody["user_id"];
        int articleId = requestBody["article_id"];
        int reportThreshold = 3;
        auto dbConnection = dbConn->getConnection();
        std::unique_ptr<sql::PreparedStatement> insertReportStmt(
            dbConnection->prepareStatement("INSERT INTO reports (article_id, user_id) VALUES (?, ?)")
        );
        insertReportStmt->setInt(1, articleId);
        insertReportStmt->setInt(2, userId);
        insertReportStmt->execute();
        std::cout << "[NewsController] Report inserted for article_id=" << articleId << ", user_id=" << userId << std::endl;
        std::unique_ptr<sql::PreparedStatement> countReportsStmt(
            dbConnection->prepareStatement("SELECT COUNT(*) as count FROM reports WHERE article_id = ?")
        );
        countReportsStmt->setInt(1, articleId);
        std::unique_ptr<sql::ResultSet> reportCountResult(countReportsStmt->executeQuery());
        int reportCount = 0;
        if (reportCountResult->next()) {
            reportCount = reportCountResult->getInt("count");
        }
        std::cout << "[NewsController] Article " << articleId << " has " << reportCount << " reports" << std::endl;
        if (reportCount >= reportThreshold) {
            std::cout << "[NewsController] Article " << articleId << " reached report threshold but hiding is disabled" << std::endl;
        }
        UserRepository userRepository(dbConn);
        NotificationRepository notificationRepository(dbConn);
        auto adminUsers = userRepository.getAllAdmins();
        std::string articleTitle;
        try {
            std::unique_ptr<sql::PreparedStatement> getTitleStmt(
                dbConnection->prepareStatement("SELECT title FROM articles WHERE id = ?")
            );
            getTitleStmt->setInt(1, articleId);
            std::unique_ptr<sql::ResultSet> articleResult(getTitleStmt->executeQuery());
            if (articleResult->next()) {
                articleTitle = articleResult->getString("title");
            }
        } catch (const std::exception& e) {
            std::cerr << "[NewsController] Error fetching article title: " << e.what() << std::endl;
        }
        for (const auto& admin : adminUsers) {
            Notification notification;
            notification.userId = admin.id;
            notification.type = "report";
            notification.articleId = articleId;
            notification.message = "Article reported: '" + articleTitle + "' (ID: " + std::to_string(articleId) + ")";
            notificationRepository.createNotification(notification);
        }
        std::cout << "[NewsController] Admins notified for reported article " << articleId << std::endl;
        std::cout << "[NewsController] reportArticle completed successfully" << std::endl;
        return crow::response(200, "Report submitted successfully");
    } catch (const std::exception& e) {
        std::cerr << "[NewsController] reportArticle error: " << e.what() << std::endl;
        return crow::response(500, "Failed to report article");
    }
}