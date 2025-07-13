#ifndef NEWSAPIADAPTERFACTORY_H
#define NEWSAPIADAPTERFACTORY_H

#include "utils/INewsApiAdapter.h"
#include "models/inc/ExternalServer.h"
#include <memory>
#include <string>

class NewsApiAdapterFactory {
public:
    static std::unique_ptr<INewsApiAdapter> createAdapter(const ExternalServer& server);
    static std::string getAdapterType(const std::string& serverName);
};

#endif 