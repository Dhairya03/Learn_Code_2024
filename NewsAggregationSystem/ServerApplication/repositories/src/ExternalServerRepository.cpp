#include "repositories/inc/ExternalServerRepository.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

ExternalServerRepository::ExternalServerRepository(std::shared_ptr<DBConnection> db) : db(std::move(db)) {}

std::vector<ExternalServer> ExternalServerRepository::getAllServers() {
    std::vector<ExternalServer> servers;
    if (!db || !db->isConnected()) return servers;

    auto conn = db->getConnection();
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement("SELECT id, name, last_accessed, status FROM external_servers")
    );
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

    while (res->next()) {
        ExternalServer server;
        server.id = res->getInt("id");
        server.name = res->getString("name");
        server.lastAccessed = res->isNull("last_accessed") ? "N/A" : res->getString("last_accessed");
        server.isActive = res->getBoolean("status");
        servers.push_back(server);
    }

    return servers;
}

std::vector<ExternalServer> ExternalServerRepository::getAllServerDetails() {
    std::vector<ExternalServer> servers;
    if (!db || !db->isConnected()) return servers;

    auto conn = db->getConnection();
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement("SELECT id, name, api_key FROM external_servers")
    );
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

    while (res->next()) {
        ExternalServer server;
        server.id = res->getInt("id");
        server.name = res->getString("name");
        server.apiKey = res->getString("api_key");
        servers.push_back(server);
    }

    return servers;
}

bool ExternalServerRepository::updateApiKey(int id, const std::string& newKey) {
    if (!db || !db->isConnected()) return false;
    auto conn = db->getConnection();
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement("UPDATE external_servers SET api_key = ? WHERE id = ?")
    );
    stmt->setString(1, newKey);
    stmt->setInt(2, id);
    return stmt->executeUpdate() > 0;
}
