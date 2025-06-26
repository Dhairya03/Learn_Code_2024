#include "../inc/Client.h"
#include "httplib.h"

Client::Client(const std::string& url) : baseUrl(url) {}

std::string Client::post(const std::string& endpoint, const std::string& bodyJson) {
    httplib::Client cli(baseUrl.c_str());
    auto res = cli.Post(endpoint.c_str(), bodyJson, "application/json");
    return res && res->status == 200 ? res->body : "{\"success\":false,\"message\":\"Server error\"}";
}

std::string Client::get(const std::string& endpoint) {
    httplib::Client cli(baseUrl.c_str());
    auto res = cli.Get(endpoint.c_str());
    return res && res->status == 200 ? res->body : "{\"success\":false,\"message\":\"Server error\"}";
}
