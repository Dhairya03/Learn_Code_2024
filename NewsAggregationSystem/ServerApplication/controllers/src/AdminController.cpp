#include "controllers/inc/AdminController.h"
#include "services/inc/AdminService.h"
#include <lib/json/json.hpp>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <iostream>

using json = nlohmann::json;

crow::response AdminController::getExternalServers(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[AdminController] getExternalServers called" << std::endl;
    try {
        AdminService service(dbConn);
        auto servers = service.getExternalServers();
        json result = json::array();
        for (const auto& s : servers) {
            result.push_back({
                {"id", s.id},
                {"name", s.name},
                {"status", s.isActive ? "Active" : "Not Active"},
                {"last_accessed", s.lastAccessed}
            });
        }
        json response = {
            {"success", true},
            {"message", "External servers fetched successfully"},
            {"data", result}
        };
        std::cout << "[AdminController] getExternalServers success" << std::endl;
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[AdminController] getExternalServers error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}, {"data", json::array()}};
        return crow::response(500, response.dump());
    }
}

crow::response AdminController::getExternalServerDetails(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[AdminController] getExternalServerDetails called" << std::endl;
    try {
        AdminService service(dbConn);
        auto servers = service.getExternalServerDetails();
        json result = json::array();
        for (const auto& s : servers) {
            result.push_back({
                {"id", s.id},
                {"name", s.name},
                {"api_key", s.apiKey}
            });
        }
        json response = {
            {"success", true},
            {"message", "External server details fetched successfully"},
            {"data", result}
        };
        std::cout << "[AdminController] getExternalServerDetails success" << std::endl;
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[AdminController] getExternalServerDetails error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}, {"data", json::array()}};
        return crow::response(500, response.dump());
    }
}

crow::response AdminController::updateApiKey(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[AdminController] updateApiKey called" << std::endl;
    try {
        auto body = json::parse(req.body);
        if (!body.contains("id") || !body.contains("api_key")) {
            json response = {{"success", false}, {"message", "Missing id or api_key"}};
            return crow::response(400, response.dump());
        }
        int id = body["id"];
        std::string key = body["api_key"];
        AdminService service(dbConn);
        if (service.updateServerApiKey(id, key)) {
            json response = {{"success", true}, {"message", "API key updated successfully"}};
            std::cout << "[AdminController] updateApiKey success" << std::endl;
            return crow::response(200, response.dump());
        }
        json response = {{"success", false}, {"message", "Failed to update API key"}};
        return crow::response(500, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[AdminController] updateApiKey error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}

crow::response AdminController::addCategory(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[AdminController] addCategory called" << std::endl;
    try {
        auto body = json::parse(req.body);
        if (!body.contains("name")) {
            json response = {{"success", false}, {"message", "Missing category name"}};
            return crow::response(400, response.dump());
        }
        AdminService service(dbConn);
        if (service.addCategory(body["name"])) {
            json response = {{"success", true}, {"message", "Category added successfully"}};
            std::cout << "[AdminController] addCategory success" << std::endl;
            return crow::response(200, response.dump());
        }
        json response = {{"success", false}, {"message", "Failed to add category"}};
        return crow::response(500, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[AdminController] addCategory error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}

crow::response AdminController::getReportedArticles(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[AdminController] getReportedArticles called" << std::endl;
    try {
        auto conn = dbConn->getConnection();
        std::string query = R"(
            SELECT a.id, a.title, COUNT(r.id) as report_count
            FROM articles a
            LEFT JOIN reports r ON a.id = r.article_id
            GROUP BY a.id, a.title
            HAVING report_count > 0
            ORDER BY report_count DESC
        )";
        auto stmt = conn->createStatement();
        auto rs = stmt->executeQuery(query);
        nlohmann::json result = nlohmann::json::array();
        while (rs->next()) {
            result.push_back(nlohmann::json{
                {"id", rs->getInt("id")},
                {"title", rs->getString("title")},
                {"report_count", rs->getInt("report_count")}
            });
        }
        json response = {{"success", true}, {"data", result}};
        std::cout << "[AdminController] getReportedArticles success" << std::endl;
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[AdminController] getReportedArticles error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}

crow::response AdminController::hideArticle(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[AdminController] hideArticle called" << std::endl;
    try {
        auto body = nlohmann::json::parse(req.body);
        if (!body.contains("article_id")) {
            json response = {{"success", false}, {"message", "Missing article_id"}};
            return crow::response(400, response.dump());
        }
        int articleId = body["article_id"];
        auto conn = dbConn->getConnection();
        json response = {{"success", true}, {"message", "Article hiding is disabled"}};
        std::cout << "[AdminController] hideArticle success" << std::endl;
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[AdminController] hideArticle error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}

crow::response AdminController::unhideArticle(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[AdminController] unhideArticle called" << std::endl;
    try {
        auto body = nlohmann::json::parse(req.body);
        if (!body.contains("article_id")) {
            json response = {{"success", false}, {"message", "Missing article_id"}};
            return crow::response(400, response.dump());
        }
        int articleId = body["article_id"];
        auto conn = dbConn->getConnection();
        json response = {{"success", true}, {"message", "Article unhiding is disabled"}};
        std::cout << "[AdminController] unhideArticle success" << std::endl;
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[AdminController] unhideArticle error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}

crow::response AdminController::hideCategory(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[AdminController] hideCategory called" << std::endl;
    try {
        auto body = nlohmann::json::parse(req.body);
        if (!body.contains("category_id")) {
            json response = {{"success", false}, {"message", "Missing category_id"}};
            return crow::response(400, response.dump());
        }
        int categoryId = body["category_id"];
        auto conn = dbConn->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement("INSERT IGNORE INTO hidden_categories (category_id) VALUES (?)"));
        stmt->setInt(1, categoryId);
        stmt->executeUpdate();
        json response = {{"success", true}, {"message", "Category hidden successfully"}};
        std::cout << "[AdminController] hideCategory success" << std::endl;
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[AdminController] hideCategory error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}

crow::response AdminController::unhideCategory(const crow::request& req, std::shared_ptr<DBConnection> dbConn) {
    std::cout << "[AdminController] unhideCategory called" << std::endl;
    try {
        auto body = nlohmann::json::parse(req.body);
        if (!body.contains("category_id")) {
            json response = {{"success", false}, {"message", "Missing category_id"}};
            return crow::response(400, response.dump());
        }
        int categoryId = body["category_id"];
        auto conn = dbConn->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement("DELETE FROM hidden_categories WHERE category_id = ?"));
        stmt->setInt(1, categoryId);
        stmt->executeUpdate();
        json response = {{"success", true}, {"message", "Category unhidden successfully"}};
        std::cout << "[AdminController] unhideCategory success" << std::endl;
        return crow::response(200, response.dump());
    } catch (const std::exception& e) {
        std::cerr << "[AdminController] unhideCategory error: " << e.what() << std::endl;
        json response = {{"success", false}, {"message", e.what()}};
        return crow::response(500, response.dump());
    }
}
