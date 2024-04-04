include_directories(${CMAKE_SOURCE_DIR})
link_libraries(cpp_secure_containers)

add_executable(example example.cpp)
add_executable(move move.cpp)
