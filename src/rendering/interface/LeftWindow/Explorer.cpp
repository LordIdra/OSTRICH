#include "Explorer.h"

#include <rendering/interface/Fonts.h>
#include <rendering/camera/CameraTransition.h>
#include <simulation/Simulation.h>
#include <main/Bodies.h>

#include <algorithm>
#include <ios>
#include <iostream>

#include <imgui.h>
#include <string>
#include <unordered_map>
#include <utility>



namespace ScenarioExplorer {
    namespace {
        const string NAME_TEXT = ICON_MDI_FORMAT_TEXT + string(" Name");
        const string MASS_TEXT = ICON_MDI_WEIGHT + string(" Mass");

        const int NAME_WEIGHT = 180;
        const int NAME_WIDTH = 260;
        const int MASS_WEIGHT = 30;

        const unsigned int NAME_COLUMN_ID = 0;
        const unsigned int MASS_COLUMN_ID = 1;

        const ImVec2 TABLE_SIZE = ImVec2(300, 730);

        const ImGuiTableFlags EXPLORER_FLAGS = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX;

        ImGuiTableSortSpecs* sortSpecs;

        auto AddHeader() -> void {
            // Use main font
            ImGui::PushFont(Fonts::Main());

            // Freeze the top row so it doesn't disappear when scrolling
            ImGui::TableSetupScrollFreeze(0, 1);
            
            // Add name header
            ImGui::TableSetupColumn(NAME_TEXT.c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, NAME_WEIGHT, NAME_COLUMN_ID);

            // Add mass header
            ImGui::TableSetupColumn(MASS_TEXT.c_str(), ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_PreferSortDescending, MASS_WEIGHT, MASS_COLUMN_ID);

            // Indicate to imgui that this is a header row
            ImGui::TableHeadersRow();

            // Stop using main font
            ImGui::PopFont();
        }

        auto AddAllBodies(vector<string> &bodyIds) -> void {
            for (const auto &pair : Bodies::GetMassiveBodies())  { bodyIds.push_back(pair.first); }
            for (const auto &pair : Bodies::GetMasslessBodies()) { bodyIds.push_back(pair.first); }
        }

        auto AddNameSelectable(const string &id, const string &name) -> void {
            ImGui::TableNextColumn();
            if (ImGui::Selectable(Fonts::NormalizeString(Fonts::MainBig(), name, NAME_WIDTH).c_str(), id == Bodies::GetSelectedBody(), ImGuiSelectableFlags_SpanAllColumns)) {
                CameraTransition::SetTargetBody(id);
            }
        }

        auto AddMassiveMassText(const double mass) -> void {
            ImGui::TableNextColumn();
            ImGui::Text("%.2e %s", mass, "kg");
        }

        auto AddBodiesToTable(const vector<string> &bodyIds) -> void {
            ImGui::PushFont(Fonts::Data());
            for (const string &id : bodyIds) {
                Body body = Bodies::GetBody(id);
                AddNameSelectable(id, body.GetName());
                AddMassiveMassText(body.GetMass());
            }
            ImGui::PopFont();
        }

        auto CompareBodyNames(string id1, string id2) -> bool {
            // Convert id1 and id2 to uppercase - necessary because string.compare is case-sensitive
            for (char &c : id1) { c = char(toupper(c)); }
            for (char &c : id2) { c = char(toupper(c)); }

            bool comparison = id1.compare(id2) >= 0;
            if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                return comparison;
            }
            return !comparison;
        }

        auto CompareBodyMasses(const string &id1, const string &id2) -> bool {
            bool comparison = Bodies::GetBody(id1).GetMass() > Bodies::GetBody(id2).GetMass();
            if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                return comparison;
            }
            return !comparison;
        }

        auto CompareBodies(const string &id1, const string &id2) -> bool {
            if (sortSpecs->Specs->ColumnIndex == NAME_COLUMN_ID) {
                return CompareBodyNames(id1, id2);
            }
            return CompareBodyMasses(id1, id2);
        }
    }

    auto Draw() -> void {
        // Begin
        ImGui::BeginTable("scenario-explorer", 2, EXPLORER_FLAGS, TABLE_SIZE);

        // For debugging purposes, will be removed later
        ImGui::ShowDemoWindow();

        // Explorer header
        AddHeader();
        //ImGui::Separator();

        // Sort body ids according to the columns the user has selected
        sortSpecs = ImGui::TableGetSortSpecs();
        vector<string> bodyIds;
        AddAllBodies(bodyIds);
        std::sort(bodyIds.begin(), bodyIds.end(), CompareBodies);

        // Add said body ids to the table
        AddBodiesToTable(bodyIds);

        // End
        ImGui::EndTable();
    }
}