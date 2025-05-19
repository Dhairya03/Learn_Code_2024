#ifndef IHTTPCLIENT_H
#define IHTTPCLIENT_H

#include <string>
#include <optional>

class IHttpClient {
public:
    virtual ~IHttpClient() = default;
    virtual std::optional<std::string> get(const std::string& url) const = 0;
};

#endif
