cmake_minimum_required(VERSION 3.5)
project(poc_load_dyn_library C CXX)
set(CMAKE_CXX_STANDARD 17)

add_library(MagicNumber SHARED STATIC magic_number.cpp)

set(TESTSKETCH_SOURCE_FILES TestSketch.c)

add_library(TestSketch SHARED  ${TESTSKETCH_SOURCE_FILES})
target_link_libraries(TestSketch MagicNumber)

add_executable(main main.cpp SketchFactory.cpp)