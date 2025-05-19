#ifndef MOCK_HTTPCIENT_H
#define MOCK_HTTPCIENT_H
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "HttpClient.h"

class MockHttpClient {
    public:
        MOCK_METHOD(std::string, sendRequest, (const std::string& url, const std::string& method, const std::string& data), ());
    };

#endif