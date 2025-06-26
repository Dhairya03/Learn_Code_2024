#include "../../repositories/inc/CategoryRepository.h"
#include <cppconn/prepared_statement.h>

CategoryRepository::CategoryRepository(std::shared_ptr<DBConnection> db) : db(std::move(db)) {}

bool CategoryRepository::addCategory(const std::string& name) {
    if (!db || !db->isConnected()) return false;

    auto conn = db->getConnection();
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement("INSERT INTO categories (name) VALUES (?)")
    );
    stmt->setString(1, name);
    return stmt->executeUpdate() > 0;
}
