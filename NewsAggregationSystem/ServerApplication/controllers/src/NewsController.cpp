#include "controllers/inc/NewsController.h"
#include "services/inc/NewsService.h"
#include "lib/json/json.hpp"
#include <iostream>
#include <string>

using json = nlohmann::json;

crow::response NewsController::getAll(const crow::request&, std::shared_ptr<DBConnection> dbConn) {
    NewsService service(dbConn);
    auto articles = service.getAllArticles();

    json result = json::array();
    for (const auto& a : articles) {
        result.push_back({
            {"id", a.id},
            {"title", a.title},
            {"description", a.description},
            {"url", a.url},
            {"source", a.source},
            {"published_at", a.publishedAt}
        });
    }

    return crow::response(200, result.dump());
}

crow::response NewsController::getByCategory(const crow::request&, std::shared_ptr<DBConnection> dbConn, const std::string& categoryName) {
    NewsService service(dbConn);
    auto articles = service.getArticlesByCategory(categoryName);

    if (articles.empty()) {
        return crow::response(404, "No articles found in this category");
    }

    nlohmann::json result = nlohmann::json::array();
    for (const auto& a : articles) {
        result.push_back({
            {"id", a.id},
            {"title", a.title},
            {"description", a.description},
            {"url", a.url},
            {"source", a.source},
            {"published_at", a.publishedAt}
        });
    }

    return crow::response(200, result.dump());
}

crow::response NewsController::search(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    auto params = crow::query_string(req.url_params);

    const char* q     = params.get("q");
    const char* start = params.get("start");
    const char* end   = params.get("end");
    const char* sort  = params.get("sort");

    if (!q) return crow::response(400, "Missing search query");

    NewsService service(dbConn);
    auto articles = service.searchArticles(
        std::string(q),
        start ? std::string(start) : "",
        end ? std::string(end) : "",
        sort ? std::string(sort) : ""
    );

    nlohmann::json result = nlohmann::json::array();
    for (const auto& a : articles) {
        result.push_back({
            {"id", a.id},
            {"title", a.title},
            {"description", a.description},
            {"url", a.url},
            {"source", a.source},
            {"published_at", a.publishedAt},
            {"likes", a.likes},
            {"dislikes", a.dislikes}
        });
    }

    return crow::response(200, result.dump());
}

crow::response NewsController::getTodayNews(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    NewsService newsService(dbConn);
    auto articles = newsService.getArticlesToday();

    json result = json::array();
    for (const auto& a : articles) {
        result.push_back({
            {"id", a.id},
            {"title", a.title},
            {"description", a.description},
            {"url", a.url},
            {"source", a.source},
            {"published_at", a.publishedAt},
            {"category", a.categoryName}
        });
    }

    return crow::response(200, result.dump());
}

crow::response NewsController::getNewsByDateAndCategory(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    auto queryParams = crow::query_string(req.url_params);
    std::string start = req.url_params.get("start") ? req.url_params.get("start") : "";
    std::string end = req.url_params.get("end") ? req.url_params.get("end") : "";
    std::string category = req.url_params.get("category") ? req.url_params.get("category") : "";

    if (start.empty() || end.empty()) {
        return crow::response(400, "Missing start or end date");
    }

    NewsService newsService(dbConn);
    auto articles = newsService.getNewsByDateAndCategory(start, end, category);

    json result = json::array();
    for (const auto& a : articles) {
        result.push_back({
            {"id", a.id},
            {"title", a.title},
            {"description", a.description},
            {"url", a.url},
            {"source", a.source},
            {"published_at", a.publishedAt},
            {"category", a.categoryId},
            {"likes", a.likes},
            {"dislikes", a.dislikes}
        });
    }
    // if (result.empty()) {
    //     return crow::response(404, "No articles found for the given date range and category");
    // }
    // else{
    //     std::cout << "Articles found: " << result.size() << std::endl;
        
    // }
    return crow::response(200, result.dump());
}

crow::response NewsController::searchNews(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    try {
        std::string query = req.url_params.get("q") ? req.url_params.get("q") : "";
        std::string startDate = req.url_params.get("start_date") ? req.url_params.get("start_date") : "";
        std::string endDate = req.url_params.get("end_date") ? req.url_params.get("end_date") : "";
        std::string sort = req.url_params.get("sort") ? req.url_params.get("sort") : "";

        NewsService service(dbConn);
        auto articles = service.searchArticles(query, startDate, endDate, sort);

        json response = json::array();
        for (const auto& article : articles) {
            json articleJson;
            articleJson["id"] = article.id;
            articleJson["title"] = article.title;
            articleJson["description"] = article.description;
            articleJson["url"] = article.url;
            articleJson["source"] = article.source;
            articleJson["publishedAt"] = article.publishedAt;
            articleJson["categoryId"] = article.categoryId;
            articleJson["categoryName"] = article.categoryName;
            articleJson["likes"] = article.likes;
            articleJson["dislikes"] = article.dislikes;
            response.push_back(articleJson);
        }
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "Error in searchNews: " << e.what() << std::endl;
        return crow::response(500, "Internal server error");
    }
}