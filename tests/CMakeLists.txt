include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  dont-use-script_tests
  function_test.cpp
  types_test.cpp
  loop_and_branch_test.cpp
  escape_string_test.cpp
  array_test.cpp
  expressions/arithmetic_expressions_test.cpp
  expressions/string_arithmetic_expressions_test.cpp
  expressions/index_expressions_test.cpp
  expressions/array_arithmetic_expression_test.cpp
)

target_link_libraries(
  dont-use-script_tests
  dont_use_script
  GTest::gtest_main
)

target_include_directories(dont-use-script_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(dont-use-script_tests)