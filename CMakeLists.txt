cmake_minimum_required(VERSION 3.6)
project(synth_firmware_game_project C CXX)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

add_library(machinemodel SHARED STATIC shared/model/machinemodel.cpp)

include(cmake_declare_and_fetch.cmake.in)

# ********* x86 teensy cores :: main branch
set(BUILD_DYNAMIC_LIBRARY ON)
DeclareAndFetch(teensy_x86_stubs https://github.com/newdigate/teensy-x86-stubs.git main src)
include_directories(${teensy_x86_stubs_SOURCE_DIR}/src)
#***********

include_directories(ext/imgui)
DeclareAndFetch(ImGuiFileDialog https://github.com/aiekick/ImGuiFileDialog.git master "/")
include_directories(${imguifiledialog_SOURCE_DIR})

include(opengl.cmake.in)
add_subdirectory(libraries/Tlc5940)
add_subdirectory(ext/imgui)
add_subdirectory(examples/blink)
add_subdirectory(examples/tlc5940)
add_subdirectory(src)

