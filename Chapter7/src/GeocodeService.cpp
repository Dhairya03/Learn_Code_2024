#include "GeocodeService.h"
#include "json.hpp"
#include <algorithm>

using json = nlohmann::json;

GeocodingService::GeocodingService(const std::string &apiKey, IHttpClient *httpClient)
    : apiKey(apiKey), httpClient(httpClient) {}

std::optional<Coordinates> GeocodingService::getCoordinates(const std::string &place, std::string &errorMessage) const
{
    std::string encodedPlace = place;
    std::replace(encodedPlace.begin(), encodedPlace.end(), ' ', '+');

    std::string url = "http://api.openweathermap.org/geo/1.0/direct?q=" + encodedPlace + "&limit=1&appid=" + apiKey;

    auto responseOpt = httpClient->get(url);
    if (!responseOpt.has_value())
        return std::nullopt;

    try
    {
        json jsonResponse = json::parse(responseOpt.value());

        if (!jsonResponse.is_array() || jsonResponse.empty())
            return std::nullopt;

        Coordinates coord;
        coord.lat = jsonResponse[0]["lat"];
        coord.lon = jsonResponse[0]["lon"];
        return coord;
    }
    catch (...)
    {
        return std::nullopt;
    }
}
