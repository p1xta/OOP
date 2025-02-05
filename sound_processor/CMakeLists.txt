cmake_minimum_required(VERSION 3.14)
set(CXX_STANDARD 17)
project(Sound_processor)

add_executable(sound_processor src/main.cpp src/sound_processor.cpp include/exceptions.hpp)