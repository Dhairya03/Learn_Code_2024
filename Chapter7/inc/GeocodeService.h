#ifndef GEOCODE_SERVICE_H
#define GEOCODE_SERVICE_H

#include <string>
#include <optional>
#include "IHttpClient.h"

struct Coordinates {
    double lat;
    double lon;
};

class GeocodingService {
public:
    GeocodingService(const std::string& apiKey, IHttpClient* httpClient);
    std::optional<Coordinates> getCoordinates(const std::string& place, std::string& errorMessage) const;


private:
    std::string apiKey;
    IHttpClient* httpClient;
};

#endif 
