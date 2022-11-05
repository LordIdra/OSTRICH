#include "BottomRightWindow.h"
#include "rendering/interface/Fonts.h"

#include <main/Simulation.h>
#include <main/Scenarios.h>

#include <imgui.h>



namespace BottomRightWindow {
    namespace {
        const ImVec2 WINDOW_SIZE = ImVec2(200, 100);
        const ImVec2 WINDOW_POSITION = ImVec2(1665, 950);

        const ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        bool windowOpen = true;
    }

    auto Draw() -> void {
        ImGui::Begin("Bottom Right Window", &windowOpen, WINDOW_FLAGS);
        ImGui::SetWindowSize(WINDOW_SIZE);
        ImGui::SetWindowPos(WINDOW_POSITION);

        ImGui::PushFont(Fonts::Main());

        ImGui::PopFont();

        ImGui::End();
    }
}