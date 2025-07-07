#include "../../repositories/inc/CategoryRepository.h"
#include <cppconn/prepared_statement.h>
#include <iostream>

CategoryRepository::CategoryRepository(std::shared_ptr<DBConnection> db) : db(std::move(db)) {}

bool CategoryRepository::addCategory(const std::string& name) {
    std::cout << "[CategoryRepository] addCategory called" << std::endl;
    if (!db || !db->isConnected()) return false;
    try {
        auto conn = db->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("INSERT INTO categories (name) VALUES (?)")
        );
        stmt->setString(1, name);
        bool result = stmt->executeUpdate() > 0;
        std::cout << "[CategoryRepository] addCategory success" << std::endl;
        return result;
    } catch (const std::exception& e) {
        std::cerr << "[CategoryRepository] addCategory error: " << e.what() << std::endl;
        return false;
    }
}
