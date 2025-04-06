cmake_minimum_required(VERSION 3.6)
project(example_blink C CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(${teensy_x86_stubs_local_include})
include_directories(../../libraries/Tlc5940)
add_library(tlc5940_example SHARED tlc5940_example.cpp)
target_link_libraries(tlc5940_example teensy_x86_stubs tlc5940)