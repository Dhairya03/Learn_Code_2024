#ifndef CURL_HTTP_CLIENT_H
#define CURL_HTTP_CLIENT_H

#include "IHttpClient.h"
#include <string>
#include <optional>

class CurlHttpClient : public IHttpClient {
public:
    std::optional<std::string> get(const std::string& url) const override;
};

#endif 
