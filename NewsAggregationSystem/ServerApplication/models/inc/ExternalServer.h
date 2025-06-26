#ifndef EXTERNAL_SERVER_H
#define EXTERNAL_SERVER_H

#include <string>
#include <vector>
#include <memory>

class ExternalServer {
public:
    int id;
    std::string name;
    std::string baseUrl;
    std::string apiKey;
    std::string lastAccessed;
    bool isActive;
};

#endif