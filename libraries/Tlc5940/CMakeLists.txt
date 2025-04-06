cmake_minimum_required(VERSION 3.10)
project(tlc5940 C CXX)
set(CMAKE_CXX_STANDARD 11)

add_library(tlc5940 SHARED  Tlc5940.cpp)
target_link_libraries(tlc5940 teensy_x86_stubs)