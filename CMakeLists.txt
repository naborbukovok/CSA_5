cmake_minimum_required(VERSION 3.16)
project(CSA_5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

add_executable(CSA_5 main.cpp book.cpp)
