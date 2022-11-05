#include "Interface.h"
#include "rendering/interface/Fonts.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <rendering/interface/Explorer.h>
#include <rendering/interface/OrbitPaths.h>
#include <rendering/interface/MassiveRender.h>
#include <rendering/interface/icons/Icons.h>

#include <glad/glad.h>


namespace Interface {

    auto Init() -> void {
        Fonts::Init();
        MassiveRender::Init();
        OrbitPaths::Init();
        Icons::Init();
    }
    
    auto Update(const double deltaTime) -> void {
        OrbitPaths::Update();
        MassiveRender::Update(deltaTime);
        Icons::DrawIcons();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ScenarioExplorer::Update();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}