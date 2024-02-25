cmake_minimum_required(VERSION 3.6)
project(example_blink C CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(${teensy_x86_stubs_local_include})
add_library(Blink SHARED  blink.cpp)
target_link_libraries(Blink teensy_x86_stubs)