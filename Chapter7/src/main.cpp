#include "GeocodeService.h"
#include <iostream>

// int main() {
//     std::string place;
//     std::cout << "Enter the place name: ";
//     std::getline(std::cin, place);

//     const std::string apiKey = "80893f2d3e9c054d3fcd643658249b11"; // Replace with your actual API key
//     GeocodingService service(apiKey);
//     service.getCoordinates(place);

//     return 0;
// }

// src/main.cpp

#include <string>
#include "CurlHttpClient.h"

int main() {
    std::string apiKey = "80893f2d3e9c054d3fcd643658249b11"; 

    CurlHttpClient httpClient;
    GeocodingService geoService(apiKey, &httpClient);

    std::cout << "Enter the place name: ";
    std::string place;
    std::getline(std::cin, place);

    std::string error;
    auto coords = geoService.getCoordinates(place, error);

    if (coords.has_value()) {
        std::cout << "Latitude: " << coords->lat << "\n";
        std::cout << "Longitude: " << coords->lon << "\n";
    } else {
        std::cerr << "Error: " << error << "\n";
    }

    return 0;
}
