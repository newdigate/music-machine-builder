//
// Created by Nicholas Newdigate on 18/02/2024.
//

#ifndef IMGUICONTROLLER_H
#define IMGUICONTROLLER_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include "../model/machinemodel.h"
#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_impl_glfw.h"
#include "../../ext/imgui/imgui_impl_opengl3.h"
#include "../../ext/imgui/imgui_internal.h"
#include "../../ext/ImGuiFileDialog/ImGuiFileDialog.h"
#include "../../ext/ImGuiFileDialog/ImGuiFileDialogConfig.h"
#include "../sketch/SketchEngine.h"

namespace newdigate {
    namespace machine {
        namespace view {

            class ImGuiController {
            public:
                ImGuiController(sketch::SketchEngine &sketch_engine) : _sketch_engine(sketch_engine),  main_tool_active(true) {
                    newdigate::machine::machinemodel &model = newdigate::machine::machine;
                }

                void InitializeImGui(GLFWwindow *window) {
                    IMGUI_CHECKVERSION();
                    ImGui::CreateContext();
                    ImGuiIO& io = ImGui::GetIO(); (void)io;
                    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
                    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
                    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Gamepad Controls
                    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

                    // Setup Dear ImGui style
                    ImGui::StyleColorsDark();
                    //ImGui::StyleColorsLight();
                    ImGui_ImplGlfw_InitForOpenGL(window, true);
                    ImGui_ImplOpenGL3_Init( "#version 150");
                }

                void RenderDeviceInspector() {
                    // Create a window called "My First Tool", with a menu bar.
                    if (main_tool_active) {
                        ImGui::Begin("Device Inspector", &main_tool_active, ImGuiWindowFlags_MenuBar);
                        if (ImGui::BeginMenuBar())
                        {
                            if (ImGui::BeginMenu("File"))
                            {
                                if (ImGui::MenuItem("Open..", "Ctrl+O")) {
                                    IGFD::FileDialogConfig config;
                                    config.path = ".";
                                    config.countSelectionMax = 1;
                                    config.flags = ImGuiFileDialogFlags_Modal;
                                    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".dylib,.so", config);
                                }
                                if (_sketch_engine.IsRunning()) {
                                    if (ImGui::MenuItem("Unload..", "Ctrl+S")) {
                                        _sketch_engine.Unload();
                                    }
                                }
                                if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
                                if (ImGui::MenuItem("Close", "Ctrl+W"))  { main_tool_active = false; }
                                ImGui::EndMenu();
                            }
                            ImGui::EndMenuBar();
                        }
                        // display
                        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
                            if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                                if (_sketch_engine.IsRunning()) {
                                    _sketch_engine.Unload();
                                }
                                _sketch_engine.LoadSketch(filePathName.c_str());
                                // action
                            }

                            ImGuiFileDialog::Instance()->Close();
                        }

                        // Generate samples and plot them
                        float samples[100];
                        for (int n = 0; n < 100; n++)
                            samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
                        ImGui::PlotLines("Samples", samples, 100);

                        // Display contents in a scrolling region
                        ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
                        ImGui::BeginChild("Scrolling");
                        for (int n = 0; n < 50; n++)
                            ImGui::Text("%04d: Some text", n);
                        ImGui::EndChild();
                        ImGui::End();
                    }
                }

                void Render() {
                    ImGui::Render();
                    // Update and Render additional Platform Windows
                    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
                    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
                    ImGuiIO& io = ImGui::GetIO();
                    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                    {
                        GLFWwindow* backup_current_context = glfwGetCurrentContext();
                        ImGui::UpdatePlatformWindows();
                        ImGui::RenderPlatformWindowsDefault();
                        glfwMakeContextCurrent(backup_current_context);
                    }
                }

            private:
                sketch::SketchEngine &_sketch_engine;
                bool main_tool_active;
            };
        }
    }
}
#endif //IMGUICONTROLLER_H
