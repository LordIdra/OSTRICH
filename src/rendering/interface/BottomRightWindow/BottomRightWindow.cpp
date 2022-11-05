#include "BottomRightWindow.h"
#include "depend/IconsMaterialDesignIcons_c.h"
#include "rendering/interface/Fonts.h"

#include <main/Simulation.h>
#include <main/Scenarios.h>

#include <imgui.h>



namespace BottomRightWindow {
    namespace {
        const ImVec2 WINDOW_SIZE = ImVec2(275, 100);
        const ImVec2 WINDOW_POSITION = ImVec2(1590, 950);

        const ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        const string SAVE_TEXT = ICON_MDI_FOLDER_UPLOAD + string(" Save");
        const string LOAD_TEXT = ICON_MDI_FOLDER_DOWNLOAD + string(" Load");
        const string SETTINGS_TEXT = ICON_MDI_COG + string(" Settings");

        bool windowOpen = true;
    }

    auto Draw() -> void {
        ImGui::Begin("Bottom Right Window", &windowOpen, WINDOW_FLAGS);
        ImGui::SetWindowSize(WINDOW_SIZE);
        ImGui::SetWindowPos(WINDOW_POSITION);

        ImGui::PushFont(Fonts::Main());

        ImGui::Button(SAVE_TEXT.c_str());
        ImGui::SameLine();
        ImGui::Button(LOAD_TEXT.c_str());
        ImGui::SameLine();
        ImGui::Button(SETTINGS_TEXT.c_str());

        ImGui::PopFont();

        ImGui::End();
    }
}