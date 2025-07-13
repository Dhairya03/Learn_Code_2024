#include "utils/NewsApiAdapterFactory.h"
#include "utils/NewsApiOrgAdapter.h"
#include "utils/TheNewsApiAdapter.h"
#include <iostream>
#include <algorithm>

std::unique_ptr<INewsApiAdapter> NewsApiAdapterFactory::createAdapter(const ExternalServer& server) {
    std::string adapterType = getAdapterType(server.name);
    
    std::cout << "[NewsApiAdapterFactory] Creating adapter for server: " << server.name 
              << " (type: " << adapterType << ")" << std::endl;
    
    if (adapterType == "newsapi.org") {
        return std::make_unique<NewsApiOrgAdapter>(server.apiKey);
    } else if (adapterType == "thenewsapi.com") {
        return std::make_unique<TheNewsApiAdapter>(server.apiKey);
    } else {
        std::cerr << "[NewsApiAdapterFactory] Unknown adapter type: " << adapterType << std::endl;
        return nullptr;
    }
}

std::string NewsApiAdapterFactory::getAdapterType(const std::string& serverName) {
    std::string name = serverName;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    
    if (name.find("newsapi.org") != std::string::npos || 
        name.find("newsapi") != std::string::npos) {
        return "newsapi.org";
    } else if (name.find("thenewsapi.com") != std::string::npos || 
               name.find("thenewsapi") != std::string::npos) {
        return "thenewsapi.com";
    } else {
        return "newsapi.org";
    }
} 