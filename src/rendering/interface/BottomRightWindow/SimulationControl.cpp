#include "SimulationControl.h"

#include <rendering/interface/Fonts.h>
#include <main/Simulation.h>

#include <imgui.h>



namespace SimulationControl {
    namespace {
        const string SAVE_TEXT = ICON_MDI_FOLDER_UPLOAD + string(" Save");
        const string LOAD_TEXT = ICON_MDI_FOLDER_DOWNLOAD + string(" Load");
        const string SETTINGS_TEXT = ICON_MDI_COG + string(" Settings");

        const string SPEED_INDICATOR_ICON = ICON_MDI_CHEVRON_RIGHT_CIRCLE;
        const string SPEED_ICON = ICON_MDI_PLAY_SPEED;

        auto AddGeneralButtons() -> void {
            ImGui::PushFont(Fonts::Main());

            ImGui::Button(SAVE_TEXT.c_str());
            ImGui::SameLine();
            ImGui::Button(LOAD_TEXT.c_str());
            ImGui::SameLine();
            ImGui::Button(SETTINGS_TEXT.c_str());

            ImGui::PopFont();
        }

        auto AddSpeedIndicatorIcons() -> void {
            string simulationSpeedStringActive;
            string simulationSpeedStringInactive;

            for (int i = 0; i < Simulation::GetSimulationSpeedMultiplier(); i++) {
                simulationSpeedStringActive += SPEED_INDICATOR_ICON;
            }

            for (int i = 0; i < Simulation::MAX_MULTIPLIER - Simulation::GetSimulationSpeedMultiplier(); i++) {
                simulationSpeedStringInactive += SPEED_INDICATOR_ICON;
            }

            ImGui::PushFont(Fonts::Data());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 160, 255, 255));
            ImGui::Text("%s", simulationSpeedStringActive.c_str());
            ImGui::PopStyleColor();

            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(80, 80, 80, 255));
            ImGui::Text("%s", simulationSpeedStringInactive.c_str());
            ImGui::PopStyleColor();

            ImGui::PopStyleVar();
            ImGui::PopFont();
        }

        auto AddSpeedIcon() -> void {
            ImGui::Text("%s %.0f%s", SPEED_ICON.c_str(), Simulation::GetSimulationSpeed(), "s per second");
        }
    }

    auto Draw() -> void {
        AddGeneralButtons();
        AddSpeedIndicatorIcons();
        AddSpeedIcon();
    }
}