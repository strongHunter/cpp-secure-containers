cmake_minimum_required(VERSION 3.21)
project(cpp-secure-containers)

option(BUILD_EXAMPLES "Build the examples" OFF)
option(ENABLE_TESTING "Build the tests" OFF)

set(CMAKE_CXX_STANDARD 20)

add_library(cpp_sc INTERFACE)
add_library(cpp_sc::cpp_sc ALIAS cpp_sc)

target_include_directories(cpp_sc INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/cpp_sc)
target_sources(cpp_sc INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/cpp_sc/sanitizing_allocator.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/cpp_sc/vector_secure.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/cpp_sc/basic_string_secure.h>)

add_library(cpp_sc_platform STATIC src/platform.cpp)
target_include_directories(cpp_sc_platform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(cpp_sc INTERFACE cpp_sc_platform)

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
