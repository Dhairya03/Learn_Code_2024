#include "lib/crow/crow_all.h"
#include "controllers/inc/UserController.h"
#include "controllers/inc/AuthController.h"
#include "controllers/inc/NewsController.h"
#include "controllers/inc/SavedArticleController.h"
#include "controllers/inc/AdminController.h"
#include "controllers/inc/NotificationController.h"
#include "database/inc/DBConnection.h"
#include "jobs/inc/NewsFetchJob.h"
#include <thread>
#include <iostream>
#include <chrono>

void startNewsFetcher(std::shared_ptr<DBConnection> dbConn) {
    std::thread([dbConn]() {
        NewsFetchJob job(dbConn);

        while (true) {
            std::cout << "[NewsFetchJob Thread] Running at " << std::time(nullptr) << "\n";
            job.run();
       
            // std::this_thread::sleep_for(std::chrono::hours(3));
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

    startNewsFetcher(dbConn);

    std::cout << "Server running on port 8080...\n";
    app.port(8080).multithreaded().run();
}
