cmake_minimum_required(VERSION 2.8.0)

project(Test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(test main.cpp io.cpp image.cpp algo.cpp)
