#include "lib/crow/crow_all.h"
#include "controllers/inc/UserController.h"
#include "controllers/inc/AuthController.h"
#include "controllers/inc/NewsController.h"
#include "controllers/inc/SavedArticleController.h"
#include "controllers/inc/AdminController.h"
#include "controllers/inc/NotificationController.h"
#include "database/inc/DBConnection.h"
#include "jobs/inc/NewsFetchJob.h"
#include "utils/NewsApiOrgAdapter.h"
#include "utils/TheNewsApiAdapter.h"
#include <thread>
#include <iostream>
#include <chrono>

void startNewsFetcher(std::shared_ptr<DBConnection> dbConn) {
    std::thread([dbConn]() {
        std::string apiKey1 = "8f4cda27870e4de79de71ec7876d3733"; // NewsApiOrg
        std::string apiKey2 = "PLFrdIv6ewEpzWbUrL2MXBn5Z6ZtD7AAuD5rUUhY"; // TheNewsApi
        bool useFirst = true;
        while (true) {
            std::unique_ptr<INewsApiAdapter> adapter;
            if (useFirst) {
                std::cout << "[startNewsFetcher] Using NewsApiOrgAdapter" << std::endl;
                adapter = std::make_unique<NewsApiOrgAdapter>(apiKey1);
            } else {
                std::cout << "[startNewsFetcher] Using TheNewsApiAdapter" << std::endl;
                adapter = std::make_unique<TheNewsApiAdapter>(apiKey2);
            }
            NewsFetchJob job(dbConn, std::move(adapter));
            job.run();
            // Check if articles were fetched (by checking DB or logs). For now, alternate if no articles fetched.
            // In a real system, you might want to check the return value or DB state.
            // Here, we alternate if the last fetch failed (simulate by checking logs or a flag).
            // For simplicity, alternate every time for now:
            useFirst = !useFirst;
            std::this_thread::sleep_for(std::chrono::minutes(10));
        }
    }).detach();
}

int main() {
    crow::SimpleApp app;

    auto dbConn = std::make_shared<DBConnection>();
    if (!dbConn->connect("tcp://127.0.0.1:3306", "root", "Root@1234", "news_aggregator_db")) {
        std::cerr << "DB connection failed\n";
        return 1;
    }

    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return AuthController::login(req, dbConn);
    });

    CROW_ROUTE(app, "/logout").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return AuthController::logout(req, dbConn);
    });

    CROW_ROUTE(app, "/news/today").methods("GET"_method)
    ([dbConn](const crow::request& req){
        return NewsController::getTodayNews(req, dbConn);
    });
    

    CROW_ROUTE(app, "/news").methods("GET"_method)
    ([dbConn](const crow::request& req){
        return NewsController::getNewsByDateAndCategory(req, dbConn);
    });


    CROW_ROUTE(app, "/news/category/<string>").methods("GET"_method)
    ([dbConn](const crow::request& req, const std::string& categoryName) {
        return NewsController::getByCategory(req, dbConn, categoryName);
    });

    
    CROW_ROUTE(app, "/user/articles/save").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return SavedArticleController::save(req, dbConn);
    });

    CROW_ROUTE(app, "/user/articles").methods("GET"_method)
    ([dbConn](const crow::request& req) {
        return SavedArticleController::getSavedArticles(req, dbConn);
    });

    CROW_ROUTE(app, "/user/articles/<int>").methods("DELETE"_method)
    ([dbConn](const crow::request& req, int articleId) {
        return SavedArticleController::deleteSavedArticle(req, dbConn, articleId);
    });

    CROW_ROUTE(app, "/signup").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return UserController::signup(req, dbConn);  
    });

    CROW_ROUTE(app, "/admin/servers/status").methods("GET"_method)
    ([dbConn](const crow::request& req){
        return AdminController::getExternalServers(req, dbConn);
    });

    CROW_ROUTE(app, "/admin/servers/details").methods("GET"_method)
    ([dbConn](const crow::request& req){
        return AdminController::getExternalServerDetails(req, dbConn);
    });

    CROW_ROUTE(app, "/admin/servers/update").methods("POST"_method)
    ([dbConn](const crow::request& req){
        return AdminController::updateApiKey(req, dbConn);
    });

    CROW_ROUTE(app, "/admin/categories/add").methods("POST"_method)
    ([dbConn](const crow::request& req){
        return AdminController::addCategory(req, dbConn);
    });

    // Notification routes
    CROW_ROUTE(app, "/notifications").methods("GET"_method)
    ([dbConn](const crow::request& req) {
        return NotificationController::getNotifications(req, dbConn);
    });

    CROW_ROUTE(app, "/notifications/read").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return NotificationController::markNotificationAsRead(req, dbConn);
    });

    CROW_ROUTE(app, "/notifications/delete").methods("DELETE"_method)
    ([dbConn](const crow::request& req) {
        return NotificationController::deleteNotification(req, dbConn);
    });

    CROW_ROUTE(app, "/notifications/settings").methods("GET"_method)
    ([dbConn](const crow::request& req) {
        return NotificationController::getUserNotificationSettings(req, dbConn);
    });

    CROW_ROUTE(app, "/notifications/configure/category").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return NotificationController::updateCategorySettings(req, dbConn);
    });

    CROW_ROUTE(app, "/notifications/configure/keywords").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return NotificationController::updateKeywords(req, dbConn);
    });

    CROW_ROUTE(app, "/news/search").methods("GET"_method)
    ([dbConn](const crow::request& req){
        return NewsController::searchNews(req, dbConn);
    });

    CROW_ROUTE(app, "/articles/report").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return NewsController::reportArticle(req, dbConn);
    });

    CROW_ROUTE(app, "/admin/reported_articles").methods("GET"_method)
    ([dbConn](const crow::request& req) {
        return AdminController::getReportedArticles(req, dbConn);
    });

    CROW_ROUTE(app, "/admin/hide_article").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return AdminController::hideArticle(req, dbConn);
    });

    CROW_ROUTE(app, "/admin/unhide_article").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return AdminController::unhideArticle(req, dbConn);
    });

    CROW_ROUTE(app, "/admin/hide_category").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return AdminController::hideCategory(req, dbConn);
    });

    CROW_ROUTE(app, "/admin/unhide_category").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return AdminController::unhideCategory(req, dbConn);
    });

    startNewsFetcher(dbConn);

    std::cout << "Server running on port 8080...\n";
    app.port(8080).multithreaded().run();
}
