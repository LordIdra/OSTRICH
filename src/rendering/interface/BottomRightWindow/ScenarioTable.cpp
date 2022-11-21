#include "ScenarioTable.h"

#include <rendering/interface/Fonts.h>
#include <scenarios/ScenarioFileUtil.h>

#include <algorithm>
#include <imgui.h>



namespace ScenarioTable {
    namespace {
        const int NAME_WIDTH = 280;
        const int BODIES_WEIGHT = 45;
        const int TIME_WEIGHT = 100;

        const unsigned int NAME_COLUMN_ID = 0;
        const unsigned int BODIES_COLUMN_ID = 1;
        const unsigned int TIME_COLUMN_ID = 2;

        const string NAME_TEXT = ICON_MDI_FORMAT_TEXT + string(" Name");
        const string BODIES_TEXT = ICON_MDI_EARTH + string(" Bodies");
        const string TIME_TEXT = ICON_MDI_CLOCK + string(" Time");

        const ImGuiTableFlags TABLE_FLAGS = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX;

        ImGuiTableSortSpecs* sortSpecs;

        string selectedFile;

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

        auto CompareScenarioFileNames(ScenarioFile scenario1, ScenarioFile scenario2) -> bool {
            // Convert id1 and id2 to uppercase - necessary because string.compare is case-sensitive
            for (char &c : scenario1.nameWithoutExtension) { c = char(toupper(c)); }
            for (char &c : scenario2.nameWithoutExtension) { c = char(toupper(c)); }

            bool comparison = scenario1.nameWithoutExtension.compare(scenario2.nameWithoutExtension) >= 0;
            if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                return comparison;
            }
            return !comparison;
        }

        auto CompareScenarioFileBodies(const ScenarioFile &scenario1, const ScenarioFile &scenario2) -> bool {
            bool comparison = scenario1.bodyCount > scenario2.bodyCount;
            if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                return comparison;
            }
            return !comparison;
        }

        auto CompareScenarioFileTimes(const ScenarioFile &scenario1, const ScenarioFile &scenario2) -> bool {
            bool comparison = scenario1.rawTime > scenario2.rawTime;
            if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                return comparison;
            }
            return !comparison;
        }

        auto CompareScenarioFiles(const ScenarioFile &scenario1, const ScenarioFile &scenario2) -> bool {
            if (sortSpecs->Specs->ColumnIndex == NAME_COLUMN_ID) {
                return CompareScenarioFileNames(scenario1, scenario2);
            } else if (sortSpecs->Specs->ColumnIndex == BODIES_COLUMN_ID) { // NOLINT(readability-else-after-return)
                return CompareScenarioFileBodies(scenario1, scenario2);
            }
            return CompareScenarioFileTimes(scenario1, scenario2);
        }

        auto AddNameText(const string &text, const bool selectable) -> void {

            ImGui::TableNextColumn();
            if (!selectable) {
                ImGui::Text("%s", text.c_str());
                return;
            }
            
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
    }
    
    auto AddFileTable(const bool selectable) -> void {
        if (ImGui::BeginTable("file-table", 3, TABLE_FLAGS, TABLE_SIZE)) {
            AddHeader();

            sortSpecs = ImGui::TableGetSortSpecs();
            vector<ScenarioFile> scenarios = ScenarioFileUtil::GetScenarios();
            std::sort(scenarios.begin(), scenarios.end(), CompareScenarioFiles);

            ImGui::PushFont(Fonts::Data());
            for (const ScenarioFile &scenario : scenarios) {
                AddNameText(scenario.nameWithoutExtension, selectable);
                AddBodiesText(scenario.bodyCount);
                AddTimeText(scenario.formattedTime);
            }
            ImGui::PopFont();

            ImGui::EndTable();
        }
    }

    auto GetSelectedFile() -> string {
        return selectedFile;
    }
}