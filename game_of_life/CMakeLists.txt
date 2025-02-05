cmake_minimum_required(VERSION 3.14)
set(CXX_STANDARD 17)
project(Game_of_life)

option(MATCH_TESTS "build tests (or no)" ON)

if (MATCH_TESTS)
        enable_testing()
        add_subdirectory(tests)
        add_subdirectory("../googletest" "${CMAKE_BINARY_DIR}/googletest")
endif()


add_executable(life src/main.cpp src/GameOfLife.cpp)
target_link_libraries(life sfml-graphics sfml-window sfml-system)