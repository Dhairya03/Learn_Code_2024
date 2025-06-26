#include "repositories/inc/UserRepository.h"
#include "database/inc/DBConnection.h"
#include <cppconn/prepared_statement.h>
#include <iostream>

UserRepository::UserRepository(std::shared_ptr<DBConnection> dbConn) : db(std::move(dbConn)) {}

bool UserRepository::createUser(const User& user) {
    if (!db || !db->isConnected()) return false;

    try {
        auto conn = db->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("INSERT INTO users (username, email, password, role) VALUES (?, ?, ?, ?)"));

        stmt->setString(1, user.username);
        stmt->setString(2, user.email);
        stmt->setString(3, user.password); 
        stmt->setString(4, User::roleToString(user.role));

        stmt->execute();
        return true;

    } catch (const sql::SQLException& e) {
        std::cerr << "[UserRepository] createUser failed: " << e.what() << "\n";
        return false;
    }
}

std::optional<User> UserRepository::findByEmailAndPassword(const std::string& email, const std::string& password) {
    if (!db || !db->isConnected()) return std::nullopt;

    try {
        auto conn = db->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT id, username, email, role FROM users WHERE email = ? AND password = ?"));
        stmt->setString(1, email);
        stmt->setString(2, password);  

        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        if (res->next()) {
            User user;
            user.id = res->getInt("id");
            user.username = res->getString("username");
            user.email = res->getString("email");
            user.role = User::parseRole(res->getString("role"));
            return user;
        }

    } catch (const sql::SQLException& e) {
        std::cerr << "[UserRepository] Error: " << e.what() << "\n";
    }

    return std::nullopt;
}
