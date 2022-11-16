#include "BottomRightWindow.h"

#include "rendering/interface/BottomRightWindow/SimulationControl.h"
#include <util/Types.h>
#include <depend/IconsMaterialDesignIcons_c.h>


#include <imgui.h>


namespace BottomRightWindow {
    namespace {
        const ImVec2 WINDOW_SIZE = ImVec2(275, 120);
        const ImVec2 WINDOW_POSITION = ImVec2(1490, 860);

        const ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        bool windowOpen = true;
    }

    auto Draw() -> void {
        ZoneScoped;
        ImGui::Begin("Right Window", &windowOpen, WINDOW_FLAGS);
        ImGui::SetWindowSize(WINDOW_SIZE);
        ImGui::SetWindowPos(WINDOW_POSITION);

        SimulationControl::Draw();

        ImGui::End();
    }
}