#include "Interface.h"
#include "rendering/interface/Style.h"

#include <rendering/interface/Fonts.h>
#include <rendering/interface/BottomRightWindow/BottomRightWindow.h>
#include <rendering/interface/TopRightWindow/TopRightWindow.h>
#include <rendering/interface/LeftWindow/LeftWindow.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>



namespace Interface {

    auto Init() -> void {
        Style::Init();
        Fonts::Init();
    }
    
    auto Update() -> void {
        ZoneScoped;
        // Declare new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        TopRightWindow::Draw();
        BottomRightWindow::Draw();
        LeftWindow::Draw();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}