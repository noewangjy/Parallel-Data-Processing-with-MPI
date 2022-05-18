cmake_minimum_required(VERSION 3.21)
project(Project_3)

include_directories("include")

set(CMAKE_CXX_COMPILER "/usr/local/bin/mpicxx")
set(CMAKE_CXX_STANDARD 14)

add_executable(MatMul utils.hpp MatMul.cpp)
