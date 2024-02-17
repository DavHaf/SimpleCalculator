cmake_minimum_required(VERSION 3.14)
project(SimpleCalculator)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5df0241ea4880e5a846775d3efc8b873f7b36c31.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(
    equation
    equation.cpp
)

add_executable(
    calculator
    calculator.cpp
)

add_executable(
    calculator_test
    test.cpp
)

target_link_libraries(
    calculator_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(calculator_test)