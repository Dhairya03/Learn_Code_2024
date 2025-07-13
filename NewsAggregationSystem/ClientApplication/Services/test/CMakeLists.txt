cmake_minimum_required(VERSION 3.10)
project(ClientServiceTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/../inc
    ${CMAKE_SOURCE_DIR}/../src
    ${GTEST_INCLUDE_DIRS}
)

add_executable(test_ArticleService test_ArticleService.cpp)
target_link_libraries(test_ArticleService GTest::gtest GTest::gtest_main pthread)

add_executable(test_SavedArticleService test_SavedArticleService.cpp)
target_link_libraries(test_SavedArticleService GTest::gtest GTest::gtest_main pthread)

add_executable(test_NotificationService test_NotificationService.cpp)
target_link_libraries(test_NotificationService GTest::gtest GTest::gtest_main pthread)

add_executable(test_AuthService test_AuthService.cpp)
target_link_libraries(test_AuthService GTest::gtest GTest::gtest_main pthread)

# Enable testing
enable_testing()

add_test(NAME ArticleServiceTest COMMAND test_ArticleService)
add_test(NAME SavedArticleServiceTest COMMAND test_SavedArticleService)
add_test(NAME NotificationServiceTest COMMAND test_NotificationService)
add_test(NAME AuthServiceTest COMMAND test_AuthService) 