cmake_minimum_required(VERSION 3.21)
project(cpp-secure-containers)

option(BUILD_EXAMPLES "Build the examples" OFF)
option(ENABLE_TESTING "Build the tests" OFF)

set(CMAKE_CXX_STANDARD 20)

add_library(cpp_secure_containers INTERFACE)
target_include_directories(cpp_secure_containers INTERFACE include)
target_sources(cpp_secure_containers INTERFACE
        include/sanitizing_allocator.h
        include/vector_secure.h
        include/basic_string_secure.h)

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
