cmake_minimum_required(VERSION 3.10)

project(NewsAggregatorServer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files from the actual directory structure
file(GLOB_RECURSE SERVER_SOURCES
    database/src/*.cpp
    controllers/src/*.cpp
    services/src/*.cpp
    repositories/src/*.cpp
    models/src/*.cpp
    jobs/src/*.cpp
    utils/src/*.cpp
)

file(GLOB_RECURSE SERVER_HEADERS
    database/inc/*.h
    controllers/inc/*.h
    services/inc/*.h
    repositories/inc/*.h
    models/inc/*.h
    jobs/inc/*.h
    utils/inc/*.h
    lib/crow/*.h
)

# Create target
add_executable(server_main main.cpp ${SERVER_SOURCES})

# Include directories based on actual structure
target_include_directories(server_main PRIVATE
    .
    database/inc
    controllers/inc
    services/inc
    repositories/inc
    models/inc
    jobs/inc
    utils/inc
    lib
)

# Find required packages
find_package(Boost REQUIRED COMPONENTS system)

# Find MySQL C++ Connector
find_library(MYSQLCPP_LIB mysqlcppconn)

# Link libraries
target_link_libraries(server_main PRIVATE 
    ${Boost_LIBRARIES}
    pthread
)

# Only link MySQL if found (optional for this project structure)
if(MYSQLCPP_LIB)
    target_link_libraries(server_main PRIVATE ${MYSQLCPP_LIB})
    target_compile_definitions(server_main PRIVATE MYSQL_AVAILABLE)
endif()

# Add compiler flags for better debugging and warnings
target_compile_options(server_main PRIVATE
    -Wall
    -Wextra
    -g
)

# Set output directory
set_target_properties(server_main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

find_package(OpenSSL REQUIRED)

target_link_libraries(server_main PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_compile_definitions(server_main PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)