cmake_minimum_required(VERSION 3.30)
project(libimousepp)

set(CMAKE_CXX_STANDARD 23)

find_package(oatpp 1.3.0 REQUIRED)
find_package(oatpp-websocket 1.3.0 REQUIRED)
find_package(oatpp-curl 1.3.0 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(libimousepp STATIC ${SOURCES})

target_link_libraries(libimousepp
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-curl
        PUBLIC oatpp::oatpp-websocket
)
