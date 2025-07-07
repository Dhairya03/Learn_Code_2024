#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
public:
    explicit Client(const std::string& baseUrl);
    std::string post(const std::string& endpoint, const std::string& bodyJson);
    std::string get(const std::string& endpoint);
    std::string delete_(const std::string& endpoint);
private:
    std::string baseUrl;
};

#endif