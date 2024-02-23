cmake_minimum_required(VERSION 3.6)
project(synth_firmware_game C CXX)
include(opengl.cmake.in)
set(CMAKE_CXX_STANDARD 17)

add_library(machinemodel SHARED STATIC model/machinemodel.cpp)

find_package(assimp REQUIRED)
include_directories(/usr/local/Cellar/assimp/5.3.1/include/)

include_directories(../ext/LearnOpenGL)
add_executable(main
        main.cpp
        view/ViewController.cpp
        view/ImGuiController.cpp
        sketch/SketchFactory.cpp
        sketch/SketchEngine.cpp
        ../ext/LearnOpenGL/stb_image_aug.c
        ../ext/imgui/imgui.cpp
        ../ext/imgui/imgui_demo.cpp
        ../ext/imgui/imgui_draw.cpp
        ../ext/imgui/imgui_tables.cpp
        ../ext/imgui/imgui_widgets.cpp
        ../ext/imgui/imgui_impl_glfw.cpp
        ../ext/imgui/imgui_impl_opengl3.cpp
        ../ext/ImGuiFileDialog/ImGuiFileDialog.cpp
        resources/alpine-keycap-led-intermediate.cpp)
target_link_libraries(main machinemodel  ${LIBS})

target_link_libraries(main assimp)
target_link_libraries(main -L/usr/local/Cellar/assimp/5.3.1/lib/)