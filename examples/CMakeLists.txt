add_executable(example example.cpp)
target_include_directories(example PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(example PRIVATE cpp_secure_containers)
