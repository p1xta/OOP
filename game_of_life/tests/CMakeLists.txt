cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
  life_test
  tests.cpp
  ../src/GameOfLife.cpp
)
target_link_libraries(
  life_test
  GTest::gtest_main
  sfml-graphics sfml-window sfml-system
)

include(GoogleTest)
gtest_discover_tests(life_test)