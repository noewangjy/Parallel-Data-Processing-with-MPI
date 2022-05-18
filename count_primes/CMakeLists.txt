cmake_minimum_required(VERSION 3.16)
project(count_primes)

set(CMAKE_CXX_COMPILER "/usr/local/bin/mpicxx")
set(CMAKE_CXX_STANDARD 14)

add_executable(prime_sieve prime_sieve.cpp utils.cpp)
