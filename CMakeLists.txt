cmake_minimum_required(VERSION 3.21)
project(cpp-secure-containers)

option(BUILD_EXAMPLES "Build the examples" ON)
option(ENABLE_TESTING "Build the tests" ON)

set(CMAKE_CXX_STANDARD 20)

add_library(cpp_secure_containers INTERFACE)
target_sources(cpp_secure_containers INTERFACE SecureContainers.h)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(ENABLE_TESTING)
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    option(DOWNLOAD_DEPENDENCIES "Allow the downloading and in-tree building of unmet dependencies" ON)

    enable_testing()

    include(GoogleTest)
    add_subdirectory(tests)
endif()
