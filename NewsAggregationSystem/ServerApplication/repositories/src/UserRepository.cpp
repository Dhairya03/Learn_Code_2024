#include "repositories/inc/UserRepository.h"
#include "database/inc/DBConnection.h"
#include <cppconn/prepared_statement.h>
#include <iostream>

UserRepository::UserRepository(std::shared_ptr<DBConnection> dbConn) : db(std::move(dbConn)) {}

bool UserRepository::createUser(const User& user) {
    std::cout << "[UserRepository] createUser called" << std::endl;
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
        std::cout << "[UserRepository] createUser success" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "[UserRepository] createUser error: " << e.what() << std::endl;
        return false;
    }
}

std::optional<User> UserRepository::findByEmailAndPassword(const std::string& email, const std::string& password) {
    std::cout << "[UserRepository] findByEmailAndPassword called" << std::endl;
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
            std::cout << "[UserRepository] findByEmailAndPassword success" << std::endl;
            return user;
        }

    } catch (const std::exception& e) {
        std::cerr << "[UserRepository] findByEmailAndPassword error: " << e.what() << std::endl;
    }

    return std::nullopt;
}

User UserRepository::getUserById(int userId) {
    std::cout << "[UserRepository] getUserById called" << std::endl;
    User user;
    user.id = userId;
    
    if (!db || !db->isConnected()) return user;

    try {
        auto conn = db->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT id, username, email, role FROM users WHERE id = ?"));
        stmt->setInt(1, userId);

        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        if (res->next()) {
            user.id = res->getInt("id");
            user.username = res->getString("username");
            user.email = res->getString("email");
            user.role = User::parseRole(res->getString("role"));
            std::cout << "[UserRepository] getUserById success" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "[UserRepository] getUserById error: " << e.what() << std::endl;
    }

    return user;
}

std::vector<User> UserRepository::getAllAdmins() {
    std::cout << "[UserRepository] getAllAdmins called" << std::endl;
    std::vector<User> admins;
    if (!db || !db->isConnected()) return admins;
    try {
        auto conn = db->getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT id, username, email, role FROM users WHERE role = 'admin'")
        );
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        while (res->next()) {
            User user;
            user.id = res->getInt("id");
            user.username = res->getString("username");
            user.email = res->getString("email");
            user.role = User::parseRole(res->getString("role"));
            admins.push_back(user);
        }
        std::cout << "[UserRepository] getAllAdmins success" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[UserRepository] getAllAdmins error: " << e.what() << std::endl;
    }
    return admins;
}
