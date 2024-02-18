cmake_minimum_required(VERSION 3.6)
project(synth_firmware_game C CXX)
include(opengl.cmake.in)
set(CMAKE_CXX_STANDARD 17)

add_library(machinemodel SHARED STATIC model/machinemodel.cpp)

add_executable(main
        main.cpp
        view/ViewController.cpp
        view/ImGuiController.cpp
        ../ext/imgui/imgui.cpp
        ../ext/imgui/imgui_demo.cpp
        ../ext/imgui/imgui_draw.cpp
        ../ext/imgui/imgui_tables.cpp
        ../ext/imgui/imgui_widgets.cpp
        ../ext/imgui/imgui_impl_glfw.cpp
        ../ext/imgui/imgui_impl_opengl3.cpp
        ../ext/ImGuiFileDialog/ImGuiFileDialog.cpp)
target_link_libraries(main machinemodel ${LIBS})