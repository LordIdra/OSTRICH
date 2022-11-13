#include "SaveScenario.h"
#include "scenarios/ScenarioFileUtil.h"

#include <rendering/interface/Fonts.h>
#include <rendering/interface/BottomRightWindow/ScenarioTable.h>
#include <scenarios/Scenarios.h>
#include <util/Types.h>

#include <depend/IconsMaterialDesignIcons_c.h>
#include <imgui.h>
#include <algorithm>



namespace SaveScenario {
    namespace {
        const string TITLE_TEXT = ICON_MDI_FOLDER_UPLOAD + string(" Save Scenario");

        const string ENTER_FILENAME_TEXT = ICON_MDI_CANCEL + string(" Enter filename");
        const string SAVE_TEXT = ICON_MDI_CHECK_CIRCLE_OUTLINE + string(" Save");
        const string CANCEL_TEXT = ICON_MDI_CLOSE_CIRCLE_OUTLINE + string(" Cancel");
        const string OVERWRITE_TEXT = ICON_MDI_ALERT_CIRCLE_OUTLINE + string(" Overwrite");

        const int TEXT_BUFFER_SIZE = 2048;

        const ImVec2 CANCEL_BUTTON_SIZE = ImVec2(90, 0);
        const ImVec2 SAVE_BUTTON_SIZE = ImVec2(90, 0);
        const ImVec2 ERROR_BUTTON_SIZE = ImVec2(167, 0);
        const ImVec2 OVERWRITE_BUTTON_SIZE = ImVec2(130, 0);

        const ImVec4 MODAL_BACKGROUND = ImVec4(0.1, 0.1, 0.1, 0.7);
        const ImVec4 MODAL_BORDER = ImVec4(0.1, 0.1, 0.1, 0.7);
        const ImVec4 DISABLED_BUTTON_COLOR = ImVec4(0.7, 0.7, 0.7, 1.0);
        const ImVec4 TRANSPARENT_COLOR = ImVec4(0.0, 0.0, 0.0, 0.0);
        const ImVec4 OVERWRITE_BUTTON_COLOR = ImVec4(1.0, 0.7, 0.2, 1.0);

        const ImGuiPopupFlags POPUP_FLAGS = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;

        char filename[TEXT_BUFFER_SIZE]; //NOLINT(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)

        auto AddTitle() -> void {
            ImGui::PushFont(Fonts::MainBig());
            ImGui::Text("%s", Fonts::NormalizeString(Fonts::MainBig(), TITLE_TEXT, ScenarioTable::TABLE_SIZE.x).c_str());
            ImGui::PopFont();
        }

        auto AddFilenameEntry() -> void {
            ImGui::InputTextWithHint("##", "enter filename here", (char*)filename, TEXT_BUFFER_SIZE * sizeof(char));
        }

        auto AddButtons() -> void {
            // Cancel
            if (ImGui::Button(CANCEL_TEXT.c_str(), CANCEL_BUTTON_SIZE)) {
                ImGui::CloseCurrentPopup(); 
            }

            ImGui::SameLine();

            // No filename
            if (filename[0] == '\0') {
                ImGui::PushStyleColor(ImGuiCol_Text, DISABLED_BUTTON_COLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, TRANSPARENT_COLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, TRANSPARENT_COLOR);
                ImGui::Button(ENTER_FILENAME_TEXT.c_str(), ERROR_BUTTON_SIZE);
                ImGui::PopStyleColor(3);

            // Load
            } else {
                if (ScenarioFileUtil::ScenarioExists((char*)filename)) {
                    ImGui::PushStyleColor(ImGuiCol_Text,OVERWRITE_BUTTON_COLOR);
                    if (ImGui::Button(OVERWRITE_TEXT.c_str(), OVERWRITE_BUTTON_SIZE)) {
                        Scenarios::SaveScenario((char*)filename);
                        ImGui::CloseCurrentPopup(); 
                    }
                    ImGui::PopStyleColor();
                } else {
                    if (ImGui::Button(SAVE_TEXT.c_str(), SAVE_BUTTON_SIZE)) {
                        Scenarios::SaveScenario((char*)filename);
                        ImGui::CloseCurrentPopup(); 
                    }
                }
            }
        }
    }

    auto Draw() -> void {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, MODAL_BACKGROUND);
        ImGui::PushStyleColor(ImGuiCol_Border, MODAL_BORDER);

        if (ImGui::BeginPopupModal("Save Scenario", nullptr, POPUP_FLAGS)) {
            AddTitle();
            ScenarioTable::AddFileTable();
            strncpy((char*)filename, ScenarioTable::GetSelectedFile().c_str(), TEXT_BUFFER_SIZE);
            AddFilenameEntry();
            AddButtons();
            ImGui::EndPopup();
        }

        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
}