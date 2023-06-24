#include "LeftWindow.h"

#include <rendering/interface/LeftWindow/Explorer.h>
#include <rendering/interface/LeftWindow/BodyData.h>
#include <util/Types.h>

#include <imgui.h>



namespace LeftWindow {
    namespace {
        const ImVec2 WINDOW_SIZE = ImVec2(330, 1000);
        const ImVec2 WINDOW_POSITION = ImVec2(0, 0);

        const ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        bool windowOpen = true;
    }

    auto Draw() -> void {
        ZoneScoped;
        ImGui::Begin("Left Window", &windowOpen, WINDOW_FLAGS);
        ImGui::SetWindowSize(WINDOW_SIZE);
        ImGui::SetWindowPos(WINDOW_POSITION);

        ScenarioExplorer::Draw();
        BodyData::Draw();

        ImGui::End();
    }
}