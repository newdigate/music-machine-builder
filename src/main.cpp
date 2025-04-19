#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif


#include <unistd.h>

#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_glfw.h"
#include "../ext/imgui/imgui_impl_opengl3.h"
#include "../ext/imgui/imgui_internal.h"

#include "../shared/model/machinemodel.h"
#include "view/ImGuiController.h"
#include "view/ViewController.h"
#include "sketch/SketchEngine.h"

#include <Arduino.h>

GLFWwindow *window = nullptr;

newdigate::machine::machinekey keys[16*16];
newdigate::machine::machineled leds[16*16];

int count = 0;

int main() {
    for (auto i=0; i < 16; i++) {
        for (auto j=0; j < 16; j++) {
            auto &key = keys[j*16+i];
            key.x += (i+1) * 2.5f;
            key.z += j * 2.5f;

            auto &led = leds[j*16+i];
            led.x += (i+1) * 2.5f;
            led.z += j * 2.5f;

            newdigate::machine::machine.Keys.push_back( key );
            newdigate::machine::machine.Leds.push_back( led );
        }
    }

    /* Initialize the library */
    if (!glfwInit()) {
        return 0;
    }

#ifdef __APPLE__
    /* We need to explicitly ask for a 3.2 context on OS X */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // comment this line in a release build!
#endif


    window = glfwCreateWindow(1024, 786, "DIY Music Machine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }



    glfwMakeContextCurrent(window);
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    newdigate::machine::sketch::SketchEngine sketch_engine;

    newdigate::machine::view::ViewControllerFactory factory;
    newdigate::machine::view::ViewController *view_controller = factory.create(window, &newdigate::machine::machine, 1024, 786);

    newdigate::machine::view::ImGuiController imguiController(sketch_engine);
    imguiController.InitializeImGui(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        for (auto i=0; i < 128; i++) {
            for (auto j=0; j < 128; j++) {
                newdigate::machine::machine.framebuffer[j*128+i] = std::rand() * 0xFFFF;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        imguiController.RenderDeviceInspector();

        glfwMakeContextCurrent(window);
        imguiController.Render();

        /*
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);*/

        view_controller->Update();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        count++;
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}