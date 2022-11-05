#include "Interface.h"

#include <rendering/interface/Fonts.h>
#include <rendering/interface/LeftWindow/LeftWindow.h>
#include <rendering/interface/Bodies/OrbitPaths.h>
#include <rendering/interface/Bodies/MassiveRender.h>
#include <rendering/interface/icons/Icons.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>



namespace Interface {

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

        LeftWindow::Draw();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}