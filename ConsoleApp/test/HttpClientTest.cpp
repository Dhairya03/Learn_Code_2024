#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "HttpClient.h"
#include <curl/curl.h>

class MockCurl {
public:
    MOCK_METHOD(CURL*, curl_easy_init, (), ());
    MOCK_METHOD(CURLcode, curl_easy_setopt, (CURL* curl, CURLoption option, ...), ());
    MOCK_METHOD(CURLcode, curl_easy_perform, (CURL* curl), ());
    MOCK_METHOD(void, curl_easy_cleanup, (CURL* curl), ());
    MOCK_METHOD(const char*, curl_easy_strerror, (CURLcode errornum), ());
};

class HttpClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockCurl = new MockCurl();
        httpClient = new HttpClient();
    }

    void TearDown() override {
        delete httpClient;
        delete mockCurl;
    }

    MockCurl* mockCurl;
    HttpClient* httpClient;
};

TEST_F(HttpClientTest, SendRequestGetSuccess) {
    std::string url = "http://example.com";
    std::string method = "GET";
    std::string data = "";

    EXPECT_CALL(*mockCurl, curl_easy_init())
        .WillOnce(::testing::Return(reinterpret_cast<CURL*>(mockCurl)));
    EXPECT_CALL(*mockCurl, curl_easy_setopt(::testing::_, ::testing::_, ::testing::_))
        .Times(::testing::AtLeast(1));
    EXPECT_CALL(*mockCurl, curl_easy_perform(::testing::_))
        .WillOnce(::testing::Return(CURLE_OK));
    EXPECT_CALL(*mockCurl, curl_easy_cleanup(::testing::_));

    std::string response = httpClient->sendRequest(url, method, data);
    EXPECT_FALSE(response.empty());
}

TEST_F(HttpClientTest, SendRequestPostSuccess) {
    std::string url = "http://example.com";
    std::string method = "POST";
    std::string data = R"({"key": "value"})";

    EXPECT_CALL(*mockCurl, curl_easy_init())
        .WillOnce(::testing::Return(reinterpret_cast<CURL*>(mockCurl)));
    EXPECT_CALL(*mockCurl, curl_easy_setopt(::testing::_, ::testing::_, ::testing::_))
        .Times(::testing::AtLeast(1));
    EXPECT_CALL(*mockCurl, curl_easy_perform(::testing::_))
        .WillOnce(::testing::Return(CURLE_OK));
    EXPECT_CALL(*mockCurl, curl_easy_cleanup(::testing::_));

    std::string response = httpClient->sendRequest(url, method, data);
    EXPECT_FALSE(response.empty());
}

TEST_F(HttpClientTest, SendRequestCurlError) {
    std::string url = "http://example.com";
    std::string method = "GET";
    std::string data = "";

    EXPECT_CALL(*mockCurl, curl_easy_init())
        .WillOnce(::testing::Return(reinterpret_cast<CURL*>(mockCurl)));
    EXPECT_CALL(*mockCurl, curl_easy_setopt(::testing::_, ::testing::_, ::testing::_))
        .Times(::testing::AtLeast(1));
    EXPECT_CALL(*mockCurl, curl_easy_perform(::testing::_))
        .WillOnce(::testing::Return(CURLE_COULDNT_CONNECT));
    EXPECT_CALL(*mockCurl, curl_easy_strerror(CURLE_COULDNT_CONNECT))
        .WillOnce(::testing::Return("Couldn't connect to server"));
    EXPECT_CALL(*mockCurl, curl_easy_cleanup(::testing::_));

    std::string response = httpClient->sendRequest(url, method, data);
    EXPECT_TRUE(response.empty());
}