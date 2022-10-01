#include "Control.h"

#include <input/Keys.h>
#include <input/Mouse.h>
#include <rendering/camera/Camera.h>
#include <rendering/interface/Interface.h>
#include <util/Log.h>
#include <window/Window.h>
#include <main/Render.h>
#include <main/Bodies.h>
#include <main/Simulation.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>



namespace Control {
    namespace {
        const int MAJOR_VERSION = 3;
        const int MINOR_VERSION = 3;
        const char* GLSL_VERSION = "#version 330";

        double previousTime = 0;
        double deltaTime = 0;

        auto SetVersionHints() -> void {
            // Tell OpenGL which version to use, and tell OpenGL to use the CORE profile rather than COMPATIBILITY
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        auto InitGLFW() -> void {
            // Try and initialize GLFW, if it fails log a message
            if (!glfwInit()) {
                Log(ERROR, "Failed to initialize GLFW.");
                return;
            }
            SetVersionHints();
        }

        auto InitGlad() -> void {
            // Try and initialize GLAD, if it fails log a message
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {  // NOLINT - required cast for GLAD initialization
                Log(ERROR, "Failed to initialize GLAD.");
                return;
            }

            // Depth testing makes sure that fragments closer to the camera override fragments further away
            glEnable(GL_DEPTH_TEST);
        }

        auto InitImGui() -> void {
            // Initialize Imgui
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::StyleColorsDark();

            // Initialise GLFW and OpenGL backends
            ImGui_ImplGlfw_InitForOpenGL(Window::GetWindow(), true);
            ImGui_ImplOpenGL3_Init(GLSL_VERSION);

            // Initialize input/output
            ImGuiIO& io = ImGui::GetIO(); 
            (void)io;
        }
    }

    auto Init(const bool fullscreen, const string &windowTitle) -> void {
        InitGLFW();
        Window::Init(fullscreen, windowTitle);
        InitGlad();
        Mouse::Init();
        Keys::Init();
        InitImGui(); // Must be done after mouse/keys init because mouse/keys init will overwrite whatever imgui needs to set
        Camera::Init();
        Bodies::Init();
        Simulation::Init();
    }

    auto Mainloop() -> void {
        while (!Window::ShouldClose()) {

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            deltaTime = glfwGetTime() - previousTime;
            previousTime = glfwGetTime();

            Window::Background(vec4(0.0, 0.0, 0.0, 1.0));
            Camera::AddZoomDelta(Mouse::GetScrollDelta().y);

            if (Mouse::RightButtonHeld()) {
                Camera::AddAngleDelta(Mouse::GetPositionDelta());
            }

            Bodies::Update();
            Render::Update(deltaTime);
            Interface::DrawIcons();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            Camera::Update();
            Mouse::Update();
            Keys::Update();
            glfwPollEvents();
            Window::Update();
        }
    }
}

