#include "TopRightWindow.h"

#include "rendering/interface/TopRightWindow/SimulationData.h"
#include <depend/IconsMaterialDesignIcons_c.h>

#include <imgui.h>


namespace TopRightWindow {
    namespace {
        const ImVec2 WINDOW_SIZE = ImVec2(400, 600);
        const ImVec2 WINDOW_POSITION = ImVec2(1465, 0);
        const ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        bool windowOpen = true;
    }

    auto Draw(const double deltaTime) -> void {
        ImGui::Begin("Top Right Window", &windowOpen, WINDOW_FLAGS);
        ImGui::SetWindowSize(WINDOW_SIZE);
        ImGui::SetWindowPos(WINDOW_POSITION);

        SimulationData::Draw(deltaTime);

        ImGui::End();
    }
}