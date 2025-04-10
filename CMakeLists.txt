cmake_minimum_required(VERSION 3.28)
project(imouse++ VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)

find_package(oatpp 1.3.0 REQUIRED)
find_package(oatpp-websocket 1.3.0 REQUIRED)
find_package(oatpp-curl 1.3.0 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.hpp")
add_library(imouse++ STATIC ${SOURCES})

target_link_libraries(imouse++
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-curl
        PUBLIC oatpp::oatpp-websocket
)

install(TARGETS imouse++
        ARCHIVE DESTINATION lib
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/imouse++Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/imouse++Config.cmake
        INSTALL_DESTINATION lib/cmake/imouse++
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/imouse++ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)
install(TARGETS imouse++
        EXPORT imouse++Targets
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include
)

install(EXPORT imouse++Targets
        FILE imouse++Targets.cmake
        NAMESPACE imouse::
        DESTINATION lib/cmake/imouse++
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/imouse++Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/imouse++ConfigVersion.cmake
        DESTINATION lib/cmake/imouse++
)

install(FILES ${HEADERS}
        DESTINATION include/imouse++
)
