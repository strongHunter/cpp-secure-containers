macro(compile_output_test name)
  set(additional_libs ${ARGN})
  add_executable(${name} "${name}.cpp")
  target_link_libraries(${name} PRIVATE GTest::gtest_main GTest::gmock
                        ${additional_libs})
  target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR})

  add_test(NAME ${name} COMMAND ${name})
endmacro(compile_output_test)


compile_output_test(SanitizingAllocatorTest)
compile_output_test(VectorSecureTest)
compile_output_test(StringSecureTest)
compile_output_test(StringSecureConcatenationTest)
compile_output_test(StringSecureSubstrTest)
