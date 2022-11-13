#include "LoadScenario.h"

#include <rendering/interface/BottomRightWindow/ScenarioTable.h>
#include <rendering/interface/Fonts.h>
#include <scenarios/Scenarios.h>
#include <util/Types.h>

#include <depend/IconsMaterialDesignIcons_c.h>
#include <imgui.h>
#include <algorithm>



namespace LoadScenario {
    namespace {
        const string TITLE_TEXT = ICON_MDI_FOLDER_DOWNLOAD + string(" Load Scenario");

        const string NO_SCENARIO_SELECTED_TEXT = ICON_MDI_CANCEL + string(" No scenario selected");
        const string LOAD_TEXT = ICON_MDI_CHECK_CIRCLE_OUTLINE + string(" Load");
        const string CANCEL_TEXT = ICON_MDI_CLOSE_CIRCLE_OUTLINE + string(" Cancel");

        const ImVec2 CANCEL_BUTTON_SIZE = ImVec2(90, 0);
        const ImVec2 LOAD_BUTTON_SIZE = ImVec2(90, 0);
        const ImVec2 ERROR_BUTTON_SIZE = ImVec2(220, 0);

        const ImVec4 MODAL_BACKGROUND = ImVec4(0.1, 0.1, 0.1, 0.7);
        const ImVec4 MODAL_BORDER = ImVec4(0.1, 0.1, 0.1, 0.7);
        const ImVec4 DISABLED_BUTTON_COLOR = ImVec4(0.5, 0.5, 0.5, 1.0);
        const ImVec4 TRANSPARENT_COLOR = ImVec4(0.0, 0.0, 0.0, 0.0);

        const ImGuiPopupFlags POPUP_FLAGS = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;

        bool scenarioLoaded = false;

        auto AddTitle() -> void {
            ImGui::PushFont(Fonts::MainBig());
            ImGui::Text("%s", Fonts::NormalizeString(Fonts::MainBig(), TITLE_TEXT, ScenarioTable::TABLE_SIZE.x).c_str());
            ImGui::PopFont();
        }

        auto AddButtons(const bool allowCancel) -> void {
            // Cancel
            if (allowCancel) {
                if (ImGui::Button(CANCEL_TEXT.c_str(), CANCEL_BUTTON_SIZE)) {
                    ImGui::CloseCurrentPopup(); 
                }

                ImGui::SameLine();
            }

            // No scenario selected
            if (ScenarioTable::GetSelectedFile().empty()) {
                ImGui::PushStyleColor(ImGuiCol_Text, DISABLED_BUTTON_COLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, TRANSPARENT_COLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, TRANSPARENT_COLOR);
                ImGui::Button(NO_SCENARIO_SELECTED_TEXT.c_str(), ERROR_BUTTON_SIZE);
                ImGui::PopStyleColor(3);
            
            // Load
            } else {
                if (ImGui::Button(LOAD_TEXT.c_str(), LOAD_BUTTON_SIZE)) {
                    Scenarios::LoadScenario(ScenarioTable::GetSelectedFile());
                    scenarioLoaded = true;
                    ImGui::CloseCurrentPopup();
                }
            }
        }
    }

    auto Draw(const bool allowCancel) -> void {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, MODAL_BACKGROUND);
        ImGui::PushStyleColor(ImGuiCol_Border, MODAL_BORDER);

        if (ImGui::BeginPopupModal("Load Scenario", nullptr, POPUP_FLAGS)) {
            AddTitle();
            ScenarioTable::AddFileTable();
            AddButtons(allowCancel);
            ImGui::EndPopup();
        }

        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }

    auto ScenarioLoaded() -> bool {
        return scenarioLoaded;
    }
}