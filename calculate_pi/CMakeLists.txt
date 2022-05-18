cmake_minimum_required(VERSION 3.16)
project(calculate_pi)

set(CMAKE_CXX_COMPILER "/usr/local/bin/mpicxx")
set(CMAKE_CXX_STANDARD 14)

add_executable(hello_world hello_world.cpp)
add_executable(calculate_pi calculate_pi.cpp)
