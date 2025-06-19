#include "repositories/inc/UserRepository.h"
#include "database/inc/DBConnection.h"
#include <cppconn/prepared_statement.h>

bool UserRepository::createUser(const User& user) {
    DBConnection db;
    if (!db.connect("tcp://127.0.0.1:3306", "root", "Root@1234", "news_aggregator_db"))
        return false;

    auto conn = db.getConnection();
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement("INSERT INTO users (username, email, password) VALUES (?, ?, ?)"));
    stmt->setString(1, user.username);
    stmt->setString(2, user.email);
    stmt->setString(3, user.password); 
    stmt->execute();
    return true;
}
