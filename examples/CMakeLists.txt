include_directories(${CMAKE_SOURCE_DIR})
link_libraries(cpp_secure_containers)

add_executable(autoMemorySanitizing autoMemorySanitizing.cpp)
add_executable(String_AutoMemorySanitizing String_AutoMemorySanitizing.cpp)
add_executable(move move.cpp)
