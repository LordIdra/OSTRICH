#include "Interface.h"
#include "rendering/interface/Fonts.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <rendering/interface/Explorer.h>
#include <rendering/interface/BodyData.h>
#include <rendering/interface/OrbitPaths.h>
#include <rendering/interface/MassiveRender.h>
#include <rendering/interface/icons/Icons.h>

#include <glad/glad.h>


namespace Interface {

    namespace {
        const ImVec2 WINDOW_SIZE = ImVec2(330, 1000);
        const ImVec2 WINDOW_POSITION = ImVec2(0, 0);

        const ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        bool windowOpen = true;
    }

    auto Init() -> void {
        Fonts::Init();
        Icons::Init();
        MassiveRender::Init();
        OrbitPaths::Init();
    }
    
    auto Update(const double deltaTime) -> void {
        // Update orbit paths, massive body spheres, and icons
        OrbitPaths::Update();
        MassiveRender::Update(deltaTime);
        Icons::DrawIcons();

        // Declare new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Begin window
        ImGui::Begin("Scenario Explorer", &windowOpen, WINDOW_FLAGS);
        ImGui::SetWindowSize(WINDOW_SIZE);
        ImGui::SetWindowPos(WINDOW_POSITION);

        ScenarioExplorer::Draw();
        BodyData::Draw();

        // Draw the frame
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}