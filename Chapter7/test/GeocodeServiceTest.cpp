// #include "gtest/gtest.h"
// #include "GeocodeService.h"

// TEST(GeocodingServiceTest, ValidLocationTest) {
//     const std::string apiKey = "80893f2d3e9c054d3fcd643658249b11"; // Replace with a valid key
//     GeocodingService service(apiKey);

//     auto result = service.getCoordinates("London");

//     ASSERT_TRUE(result.has_value());
//     EXPECT_NEAR(result->lat, 51.5, 2.0); // approximate
//     EXPECT_NEAR(result->lon, -0.1, 2.0); // approximate
// }

// TEST(GeocodingServiceTest, InvalidLocationTest) {
//     const std::string apiKey = "80893f2d3e9c054d3fcd643658249b11";
//     GeocodingService service(apiKey);

//     auto result = service.getCoordinates("zzzzinvalidcityname");
//     EXPECT_FALSE(result.has_value());
// }

#include <gtest/gtest.h>
#include "GeocodeService.h"
#include "IHttpClient.h"

class MockHttpClient : public IHttpClient {
public:
    std::string mockResponse;
    bool fail = false;

    std::optional<std::string> get(const std::string& url) const override {
        if (fail) return std::nullopt;
        return mockResponse;
    }
};

TEST(GeocodingServiceTest, ValidLocationReturnsCoordinates) {
    MockHttpClient mock;
    mock.mockResponse = R"([{"lat": 12.34, "lon": 56.78}])";

    GeocodingService service("dummy_api_key", &mock);
    auto coords = service.getCoordinates("London");

    ASSERT_TRUE(coords.has_value());
    EXPECT_DOUBLE_EQ(coords->lat, 12.34);
    EXPECT_DOUBLE_EQ(coords->lon, 56.78);
}

TEST(GeocodingServiceTest, EmptyArrayReturnsNoCoordinates) {
    MockHttpClient mock;
    mock.mockResponse = "[]";

    GeocodingService service("dummy_api_key", &mock);
    auto coords = service.getCoordinates("InvalidCity");

    EXPECT_FALSE(coords.has_value());
}

TEST(GeocodingServiceTest, MalformedJsonReturnsNoCoordinates) {
    MockHttpClient mock;
    mock.mockResponse = R"({bad json)";

    GeocodingService service("dummy_api_key", &mock);
    auto coords = service.getCoordinates("Broken");

    EXPECT_FALSE(coords.has_value());
}

// Test 4: Network failure (curl failure)
TEST(GeocodingServiceTest, CurlFailureReturnsNoCoordinates) {
    MockHttpClient mock;
    mock.fail = true;

    GeocodingService service("dummy_api_key", &mock);
    auto coords = service.getCoordinates("OfflineCity");

    EXPECT_FALSE(coords.has_value());
}

// Test 5: API returns object instead of array
TEST(GeocodingServiceTest, ObjectResponseReturnsNoCoordinates) {
    MockHttpClient mock;
    mock.mockResponse = R"({"cod":401,"message":"Invalid API key"})";

    GeocodingService service("dummy_api_key", &mock);
    auto coords = service.getCoordinates("AuthFail");

    EXPECT_FALSE(coords.has_value());
}
