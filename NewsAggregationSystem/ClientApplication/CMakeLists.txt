cmake_minimum_required(VERSION 3.10)

project(NewsAggregatorClient)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files from the actual directory structure
file(GLOB_RECURSE CLIENT_SOURCES
    src/*.cpp
    Core/src/*.cpp
    Menu/src/*.cpp
    Services/src/*.cpp
    Utils/src/*.cpp
)

file(GLOB_RECURSE CLIENT_HEADERS
    inc/*.h
    lib/httplib/*.h
    lib/json/*.hpp
)

# Create target
add_executable(client_main 
    main.cpp 
    Services/src/ReactionService.cpp
    Services/src/CategoryService.cpp
    ${CLIENT_SOURCES}
)

# Include directories
target_include_directories(client_main PRIVATE
    inc
    lib
    lib/httplib
    lib/json
    Core/inc
    Menu/inc
    Services/inc
    Utils/inc
)

# Find required packages for httplib
find_package(Threads REQUIRED)

# Link libraries
target_link_libraries(client_main PRIVATE 
    Threads::Threads
)

# Add compiler flags for better debugging and warnings
target_compile_options(client_main PRIVATE
    -Wall
    -Wextra
    -g
)

# Set output directory
set_target_properties(client_main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Optional: Add OpenSSL support for HTTPS (if needed)
find_package(OpenSSL)
if(OpenSSL_FOUND)
    target_link_libraries(client_main PRIVATE OpenSSL::SSL OpenSSL::Crypto)
    target_compile_definitions(client_main PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)
endif()

# Optional: Add compression support
find_package(ZLIB)
if(ZLIB_FOUND)
    target_link_libraries(client_main PRIVATE ZLIB::ZLIB)
    target_compile_definitions(client_main PRIVATE CPPHTTPLIB_ZLIB_SUPPORT)
endif()