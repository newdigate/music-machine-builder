cmake_minimum_required(VERSION 3.6)
project(synth_firmware_game C CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(../ext/LearnOpenGL)
include_directories(${teensy_x86_stubs_INCLUDE_DIR})
include_directories(/usr/local/Cellar/glm/1.0.1/include/)
include_directories(/usr/local/Cellar/assimp/5.4.3/include/)

add_executable(main
        main.cpp
        view/ViewController.cpp
        view/ImGuiController.cpp
        view/ImGuiSerialMonitor.cpp
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
        ${imguifiledialog_SOURCE_DIR}/ImGuiFileDialog.cpp
        resources/alpine-keycap-led-intermediate.cpp)
target_link_libraries(main machinemodel  ${LIBS})

target_link_libraries(main -L/usr/local/Cellar/assimp/5.4.3/lib/ assimp)
target_link_libraries(main teensy_x86_stubs)
