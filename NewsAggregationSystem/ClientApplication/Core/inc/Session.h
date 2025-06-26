#ifndef SESSION_H
#define SESSION_H

#include <string>

class Session {
public:
    void clear();
    std::string getUsername() const;
    std::string getRole() const;
    int getUserId() const;
    bool isLoggedIn() const;
    void setUserId(int id) { userId = id; }
    void setUsername(const std::string& name) { username = name; }
    void setRole(const std::string& r) { role = r; }
    void setEmail(const std::string& e) { email = e; }
    void setUser(const std::string& username, const std::string& role, int userId);

private:
    std::string username;
    std::string role;
    std::string email;  
    int userId = -1;
};

#endif