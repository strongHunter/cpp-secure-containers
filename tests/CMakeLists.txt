macro(compile_output_test name)
  set(additional_libs ${ARGN})
  add_executable(${name} "${name}.cpp")
  target_link_libraries(${name} PRIVATE GTest::gtest_main GTest::gmock
                        ${additional_libs})
  target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR})
endmacro(compile_output_test)


#compile_output_test(test1 lib1 lib2)
#add_test(NAME Test1Name COMMAND test1)
