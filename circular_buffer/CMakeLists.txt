cmake_minimum_required(VERSION 3.14)
project(circular_buffer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
  main
  main.cpp
  CircularBuffer.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  buffer_test
  tests.cpp
  CircularBuffer.cpp
)
target_link_libraries(
  buffer_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(buffer_test)