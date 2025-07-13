cmake_minimum_required(VERSION 3.10)
project(ServerRepositoryTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/../inc
    ${CMAKE_SOURCE_DIR}/../src
    ${GTEST_INCLUDE_DIRS}
)

add_executable(test_NewsArticleRepository test_NewsArticleRepository.cpp)
target_link_libraries(test_NewsArticleRepository GTest::gtest GTest::gtest_main pthread)

add_executable(test_UserRepository test_UserRepository.cpp)
target_link_libraries(test_UserRepository GTest::gtest GTest::gtest_main pthread)

add_executable(test_SavedArticleRepository test_SavedArticleRepository.cpp)
target_link_libraries(test_SavedArticleRepository GTest::gtest GTest::gtest_main pthread)

add_executable(test_NotificationRepository test_NotificationRepository.cpp)
target_link_libraries(test_NotificationRepository GTest::gtest GTest::gtest_main pthread)

add_executable(test_CategoryRepository test_CategoryRepository.cpp)
target_link_libraries(test_CategoryRepository GTest::gtest GTest::gtest_main pthread)

add_executable(test_ExternalServerRepository test_ExternalServerRepository.cpp)
target_link_libraries(test_ExternalServerRepository GTest::gtest GTest::gtest_main pthread)

# Enable testing
enable_testing()

add_test(NAME NewsArticleRepositoryTest COMMAND test_NewsArticleRepository)
add_test(NAME UserRepositoryTest COMMAND test_UserRepository)
add_test(NAME SavedArticleRepositoryTest COMMAND test_SavedArticleRepository)
add_test(NAME NotificationRepositoryTest COMMAND test_NotificationRepository)
add_test(NAME CategoryRepositoryTest COMMAND test_CategoryRepository)
add_test(NAME ExternalServerRepositoryTest COMMAND test_ExternalServerRepository) 