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

#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_glfw.h"
#include "../ext/imgui/imgui_impl_opengl3.h"
#include "../ext/imgui/imgui_internal.h"
#include "../ext/ImGuiFileDialog/ImGuiFileDialog.h"
#include "../ext/ImGuiFileDialog/ImGuiFileDialogConfig.h"

#include "model/machinemodel.h"

GLFWwindow *window = nullptr;

class ImGuiController {
public:
    ImGuiController() : main_tool_active(true) {
        newdigate::machine::machinemodel &model = machine;
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
                        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", config);
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
    bool main_tool_active;
};

int main()
{
    /* Initialize the library */
    if (!glfwInit()) {
        return false;
    }

#ifdef __APPLE__
    /* We need to explicitly ask for a 3.2 context on OS X */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // comment this line in a release build!
#endif

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    window = glfwCreateWindow(640, 480, "DIY Music Machine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    ImGuiController imguiController;
    imguiController.InitializeImGui(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        imguiController.RenderDeviceInspector();

        glfwMakeContextCurrent(window);


        imguiController.Render();

        glClear(GL_COLOR_BUFFER_BIT);
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}