#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>

class HttpClient {
public:
    static std::string sendRequest(const std::string& url, const std::string& method, const std::string& data = "");
};

#endif