#include "lib/crow/crow_all.h"
#include "controllers/inc/UserController.h"
#include "controllers/inc/AuthController.h"
#include "controllers/inc/NewsController.h"
#include "controllers/inc/SavedArticleController.h"
#include "controllers/inc/AdminController.h"
#include "controllers/inc/NotificationController.h"
#include "controllers/inc/ReactionController.h"
#include "controllers/inc/PersonalizationController.h"
#include "controllers/inc/CategoryController.h"
#include "controllers/inc/CategoryController.h"
#include "database/inc/DBConnection.h"
#include "jobs/inc/NewsFetchJob.h"
#include "jobs/inc/MultiApiNewsFetchJob.h"
#include "utils/NewsApiOrgAdapter.h"
#include "utils/TheNewsApiAdapter.h"
#include <thread>
#include <iostream>
#include <chrono>

void startNewsFetcher(std::shared_ptr<DBConnection> dbConn) {
    std::thread([dbConn]() {
        while (true) {
            std::cout << "[startNewsFetcher] Starting multi-API news fetch job..." << std::endl;
            MultiApiNewsFetchJob job(dbConn);
            job.run();
            std::cout << "[startNewsFetcher] Multi-API fetch completed, sleeping for 3 hours..." << std::endl;
            std::this_thread::sleep_for(std::chrono::hours(3));
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

    CROW_ROUTE(app, "/articles/like").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return ReactionController::likeArticle(req, dbConn);
    });

    CROW_ROUTE(app, "/articles/dislike").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return ReactionController::dislikeArticle(req, dbConn);
    });

    CROW_ROUTE(app, "/articles/reaction/remove").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return ReactionController::removeReaction(req, dbConn);
    });

    CROW_ROUTE(app, "/articles/reaction/user").methods("GET"_method)
    ([dbConn](const crow::request& req) {
        return ReactionController::getUserReaction(req, dbConn);
    });

    CROW_ROUTE(app, "/articles/reactions").methods("GET"_method)
    ([dbConn](const crow::request& req) {
        return ReactionController::getArticleReactions(req, dbConn);
    });

    CROW_ROUTE(app, "/articles/personalized").methods("GET"_method)
    ([dbConn](const crow::request& req) {
        return PersonalizationController::getPersonalizedArticles(req, dbConn);
    });

    CROW_ROUTE(app, "/articles/recommended").methods("GET"_method)
    ([dbConn](const crow::request& req) {
        return PersonalizationController::getRecommendedArticles(req, dbConn);
    });

    CROW_ROUTE(app, "/articles/trending").methods("GET"_method)
    ([dbConn](const crow::request& req) {
        return PersonalizationController::getTrendingArticles(req, dbConn);
    });

    CROW_ROUTE(app, "/articles/track-view").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return PersonalizationController::trackArticleView(req, dbConn);
    });

    CROW_ROUTE(app, "/articles/track-interaction").methods("POST"_method)
    ([dbConn](const crow::request& req) {
        return PersonalizationController::trackArticleInteraction(req, dbConn);
    });

    CROW_ROUTE(app, "/user/interests").methods("GET"_method)
    ([dbConn](const crow::request& req) {
        return PersonalizationController::getUserInterests(req, dbConn);
    });

    CROW_ROUTE(app, "/user/recommended-categories").methods("GET"_method)
    ([dbConn](const crow::request& req) {
        return PersonalizationController::getRecommendedCategories(req, dbConn);
    });

    CROW_ROUTE(app, "/categories").methods("GET"_method)
    ([dbConn](const crow::request& req) {
        return CategoryController::getAllCategories(req, dbConn);
    });

    CROW_ROUTE(app, "/categories/<int>").methods("GET"_method)
    ([dbConn](const crow::request& req, int categoryId) {
        return CategoryController::getCategoryById(req, dbConn, categoryId);
    });

    CROW_ROUTE(app, "/categories/name/<string>").methods("GET"_method)
    ([dbConn](const crow::request& req, const std::string& categoryName) {
        return CategoryController::getCategoryByName(req, dbConn, categoryName);
    });

    startNewsFetcher(dbConn);

    std::cout << "Server running on port 8080...\n";
    app.port(8080).multithreaded().run();
}
