#include "Control.h"
#include "rendering/interface/BottomRightWindow/LoadScenario.h"
#include "scenarios/Scenarios.h"
#include "rendering/camera/CameraTransition.h"
#include "rendering/interface/TopRightWindow/SimulationData.h"

#include <glm/gtx/string_cast.hpp>
#include <depend/implot/implot.h>
#include <input/Keys.h>
#include <input/Mouse.h>
#include <rendering/camera/Camera.h>
#include <rendering/interface/Interface.h>
#include <rendering/world/Icons.h>
#include <rendering/world/MassiveRender.h>
#include <rendering/world/OrbitPaths.h>

#include <rendering/geometry/Rays.h>

#include <util/Log.h>
#include <window/Window.h>
#include <main/Bodies.h>
#include <simulation/Simulation.h>

#include <string>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>



namespace Control {
    namespace {
        const int MAJOR_VERSION = 3;
        const int MINOR_VERSION = 3;
        const char* GLSL_VERSION = "#version 330";

        const vec4 WINDOW_BACKGROUND = vec4(0.0, 0.0, 0.0, 1.0);

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
            glfwWindowHint(GLFW_SAMPLES, 4);
        }

        auto InitGlad() -> void {
            // Try and initialize GLAD, if it fails log a message
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {  // NOLINT - required cast for GLAD initialization
                Log(ERROR, "Failed to initialize GLAD.");
                return;
            }

            // Multisampling makes edges smoother by interpolating pixels
            glEnable(GL_MULTISAMPLE);  

            // Depth testing makes sure that fragments closer to the camera override fragments further away
            glEnable(GL_DEPTH_TEST);

            // Texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        auto InitImGui() -> void {
            // Initialize Imgui
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImPlot::CreateContext();
            ImGui::StyleColorsDark();

            // Initialise GLFW and OpenGL backends
            ImGui_ImplGlfw_InitForOpenGL(Window::GetWindow(), true);
            ImGui_ImplOpenGL3_Init(GLSL_VERSION);

            // Initialize input/output
            ImGuiIO& io = ImGui::GetIO(); 
            (void)io;
        }
    }

    auto PreReset() -> void {
        // To be called before a new scenario is loaded
        CameraTransition::PreReset();
        Bodies::PreReset();
        MassiveRender::PreReset();
        Camera::PreReset();
        Bodies::PreReset();
        Simulation::PreReset();
        SimulationData::PreReset();
    }

    auto PostReset() -> void {
        // To be called after a new scenario is loaded
        Bodies::PostReset();
        SimulationData::PostReset();
    }

    auto Init(const bool fullscreen, const string &windowTitle) -> void {
        InitGLFW();
        Window::Init(fullscreen, windowTitle);
        InitGlad();
        Mouse::Init();
        Keys::Init();
        CameraTransition::Init();
        InitImGui(); // Must be done after mouse/keys init because mouse/keys init will overwrite whatever imgui needs to set
        Icons::Init();
        MassiveRender::Init();
        OrbitPaths::Init();
        Interface::Init();
        Camera::Init();
        Simulation::Init();
    }

    auto PromptScenarioLoad() -> void {
        while ((!LoadScenario::ScenarioLoaded()) && (!Window::ShouldClose())) {
            Window::Background(WINDOW_BACKGROUND);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::OpenPopup("Load Scenario");
            LoadScenario::Draw(false);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            Mouse::Update();
            Keys::Update();
            glfwPollEvents();
            Window::Update();
        }
    }

    auto Mainloop() -> void {
        int i = 0;
        while (!Window::ShouldClose()) {

            deltaTime = glfwGetTime() - previousTime;
            previousTime = glfwGetTime();
            
            //Simulation::FrameUpdate();
            //std::thread simulationUpdateThread(Simulation::Update, deltaTime);

            Window::Background(WINDOW_BACKGROUND);
            Camera::AddZoomDelta(Mouse::GetScrollDelta().y);

            if (Mouse::RightButtonHeld()) {
                Camera::AddAngleDelta(Mouse::GetPositionDelta());
            }
            
            Interface::Update(deltaTime);
            Camera::Update();
            CameraTransition::Update(deltaTime);
            MassiveRender::Update();
            Icons::Update();
            OrbitPaths::Update();

            Mouse::Update();
            Keys::Update();
            glfwPollEvents();
            Window::Update();

            //simulationUpdateThread.join();
            
            FrameMark;
        }
    }
}

