#include "LoadScenario.h"

#include <rendering/interface/Fonts.h>
#include <main/Scenarios.h>
#include <util/Types.h>

#include <depend/IconsMaterialDesignIcons_c.h>
#include <imgui.h>
#include <algorithm>



namespace LoadScenario {
    namespace {
        const string TITLE_TEXT = ICON_MDI_FOLDER_DOWNLOAD + string(" Load Scenario");
        const string NAME_TEXT = ICON_MDI_FORMAT_TEXT + string(" Name");
        const string BODIES_TEXT = ICON_MDI_EARTH + string(" Bodies");
        const string TIME_TEXT = ICON_MDI_CLOCK + string(" Time");

        const string NO_SCENARIO_SELECTED_TEXT = ICON_MDI_CANCEL + string(" No scenario selected");
        const string LOAD_TEXT = ICON_MDI_CHECK_CIRCLE_OUTLINE + string(" Load");
        const string CANCEL_TEXT = ICON_MDI_CLOSE_CIRCLE_OUTLINE + string(" Cancel");

        const int NAME_WIDTH = 280;
        const int BODIES_WEIGHT = 45;
        const int TIME_WEIGHT = 100;

        const unsigned int NAME_COLUMN_ID = 0;
        const unsigned int BODIES_COLUMN_ID = 1;
        const unsigned int TIME_COLUMN_ID = 2;

        const ImVec2 TABLE_SIZE = ImVec2(600, 400);
        const ImVec2 CANCEL_BUTTON_SIZE = ImVec2(90, 0);
        const ImVec2 LOAD_BUTTON_SIZE = ImVec2(90, 0);
        const ImVec2 ERROR_BUTTON_SIZE = ImVec2(220, 0);

        const ImVec4 MODAL_BACKGROUND = ImVec4(0.1, 0.1, 0.1, 0.7);
        const ImVec4 MODAL_BORDER = ImVec4(0.1, 0.1, 0.1, 0.7);
        const ImVec4 DISABLED_BUTTON_COLOR = ImVec4(0.5, 0.5, 0.5, 1.0);
        const ImVec4 TRANSPARENT_COLOR = ImVec4(0.0, 0.0, 0.0, 0.0);

        const ImGuiPopupFlags POPUP_FLAGS = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;
        const ImGuiTableFlags TABLE_FLAGS = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX;

        ImGuiTableSortSpecs* sortSpecs;

        string selectedFile;

        auto AddTitle() -> void {
            ImGui::PushFont(Fonts::MainBig());
            ImGui::Text("%s", Fonts::NormalizeString(Fonts::MainBig(), TITLE_TEXT, TABLE_SIZE.x).c_str());
            ImGui::PopFont();
        }

        auto CompareScenarioFileNames(Scenarios::ScenarioFile scenario1, Scenarios::ScenarioFile scenario2) -> bool {
            // Convert id1 and id2 to uppercase - necessary because string.compare is case-sensitive
            for (char &c : scenario1.nameWithExtension) { c = char(toupper(c)); }
            for (char &c : scenario2.nameWithExtension) { c = char(toupper(c)); }

            bool comparison = scenario1.nameWithExtension.compare(scenario2.nameWithExtension) >= 0;
            if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                return comparison;
            }
            return !comparison;
        }

        auto CompareScenarioFileBodies(const Scenarios::ScenarioFile &scenario1, const Scenarios::ScenarioFile &scenario2) -> bool {
            bool comparison = scenario1.bodyCount > scenario2.bodyCount;
            if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                return comparison;
            }
            return !comparison;
        }

        auto CompareScenarioFileTimes(const Scenarios::ScenarioFile &scenario1, const Scenarios::ScenarioFile &scenario2) -> bool {
            bool comparison = scenario1.rawTime > scenario2.rawTime;
            if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                return comparison;
            }
            return !comparison;
        }

        auto CompareScenarioFiles(const Scenarios::ScenarioFile &scenario1, const Scenarios::ScenarioFile &scenario2) -> bool {
            if (sortSpecs->Specs->ColumnIndex == NAME_COLUMN_ID) {
                return CompareScenarioFileNames(scenario1, scenario2);
            } else if (sortSpecs->Specs->ColumnIndex == BODIES_COLUMN_ID) { // NOLINT(readability-else-after-return)
                return CompareScenarioFileBodies(scenario1, scenario2);
            }
            return CompareScenarioFileTimes(scenario1, scenario2);
        }

        auto AddHeader() -> void {
            // Use main font
            ImGui::PushFont(Fonts::Main());

            // Freeze the top row so it doesn't disappear when scrolling
            ImGui::TableSetupScrollFreeze(0, 1);

            // Add name header
            ImGui::TableSetupColumn(NAME_TEXT.c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, NAME_WIDTH, NAME_COLUMN_ID);

            // Add mass header
            ImGui::TableSetupColumn(BODIES_TEXT.c_str(), ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_PreferSortDescending, BODIES_WEIGHT, BODIES_COLUMN_ID);

            ImGui::TableSetupColumn(TIME_TEXT.c_str(), ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_PreferSortDescending, TIME_WEIGHT, TIME_COLUMN_ID);

            // Indicate to imgui that this is a header row
            ImGui::TableHeadersRow();

            // Stop using main font
            ImGui::PopFont();
        }

        auto AddNameText(const string &text) -> void {
            ImGui::TableNextColumn();
            if (ImGui::Selectable(Fonts::NormalizeString(Fonts::MainBig(), text, NAME_WIDTH).c_str(), selectedFile == text, ImGuiSelectableFlags_SpanAllColumns)) {
                selectedFile = text;
            }
        }

        auto AddBodiesText(const int bodies) -> void {
            ImGui::TableNextColumn();
            ImGui::Text("%d", bodies);
        }

        auto AddTimeText(const string &text) -> void {
            ImGui::TableNextColumn();
            ImGui::Text("%s", text.c_str());
        }

        auto AddFileTable() -> void {
            if (ImGui::BeginTable("file-table", 3, TABLE_FLAGS, TABLE_SIZE)) {
                AddHeader();

                sortSpecs = ImGui::TableGetSortSpecs();
                vector<Scenarios::ScenarioFile> scenarios = Scenarios::GetScenarios();
                std::sort(scenarios.begin(), scenarios.end(), CompareScenarioFiles);


                ImGui::PushFont(Fonts::Data());
                for (const Scenarios::ScenarioFile &scenario : scenarios) {
                    AddNameText(scenario.nameWithExtension);
                    AddBodiesText(scenario.bodyCount);
                    AddTimeText(scenario.formattedTime);
                }
                ImGui::PopFont();

                ImGui::EndTable();
            }
        }

        auto AddButtons() -> void {
            // Cancel
            if (ImGui::Button(CANCEL_TEXT.c_str(), CANCEL_BUTTON_SIZE)) {
                ImGui::CloseCurrentPopup(); 
            }

            ImGui::SameLine();

            // No scenario selected
            if (selectedFile.empty()) {
                ImGui::PushStyleColor(ImGuiCol_Text, DISABLED_BUTTON_COLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, TRANSPARENT_COLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, TRANSPARENT_COLOR);
                ImGui::Button(NO_SCENARIO_SELECTED_TEXT.c_str(), ERROR_BUTTON_SIZE);
                ImGui::PopStyleColor(3);
            
            // Load
            } else {
                if (ImGui::Button(LOAD_TEXT.c_str(), LOAD_BUTTON_SIZE)) {
                    Scenarios::LoadScenario(selectedFile);
                    ImGui::CloseCurrentPopup(); 
                }
            }
        }
    }

    auto Draw() -> void {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, MODAL_BACKGROUND);
        ImGui::PushStyleColor(ImGuiCol_Border, MODAL_BORDER);

        if (ImGui::BeginPopupModal("Load Scenario", nullptr, POPUP_FLAGS)) {
            AddTitle();
            AddFileTable();
            AddButtons();
            ImGui::EndPopup();
        }

        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
}