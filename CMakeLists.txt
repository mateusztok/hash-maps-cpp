cmake_minimum_required(VERSION 3.20)
project(hash-maps)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

enable_testing()
add_subdirectory(tests)