#include "Explorer.h"

#include <rendering/interface/Fonts.h>
#include "main/Simulation.h"
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
        const string RADIUS_TEXT = ICON_MDI_RADIUS + string(" Radius");
        const string COLOR_TEXT = ICON_MDI_BRUSH + string(" Color");
        const string SPEED_TEXT = ICON_MDI_SPEEDOMETER_SLOW + string(" Velocity");
        const string ACCELERATION_TEXT = ICON_MDI_CHEVRON_DOUBLE_UP + string(" Acceleration");
        const string KINETIC_ENERGY_TEXT = ICON_MDI_CUBE_SEND + string(" Kinetic Energy");
        const string POTENTIAL_ENERGY_TEXT = ICON_MDI_ATOM + string(" Potential Energy");
        const string TOTAL_ENERGY_TEXT = ICON_MDI_LIGHTNING_BOLT + string(" Total Energy");

        const int NAME_WEIGHT = 180;
        const int NAME_WIDTH = 260;
        const int MASS_WEIGHT = 30;
        const int DATA_KEY_COLUMN_WIDTH = 170;
        const int DATA_VALUE_COLUMN_WIDTH =  130;

        const unsigned int NAME_COLUMN_ID = 0;
        const unsigned int MASS_COLUMN_ID = 1;

        const ImVec2 WINDOW_SIZE = ImVec2(330, 1000);
        const ImVec2 WINDOW_POSITION = ImVec2(0, 0);
        const ImVec2 EXPLORER_SIZE = ImVec2(300, 730);
        const ImVec2 BODY_DATA_SIZE = ImVec2(300, 100);

        const ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        const ImGuiTableFlags EXPLORER_FLAGS = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX;
        const ImGuiTableFlags BODY_DATA_FLAGS = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_SizingFixedFit;

        ImGuiTableSortSpecs* sortSpecs;
        
        bool windowOpen = true;

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
                Bodies::SetSelectedBody(id);
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

        auto AddExplorer() -> void {
            // Begin
            ImGui::BeginTable("scenario-explorer", 2, EXPLORER_FLAGS, EXPLORER_SIZE);

            // For debugging purposes, will be removed later
            ImGui::ShowDemoWindow();

            // Explorer header
            AddHeader();

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

        auto AddBodyDataMass(const Body &body) -> void {
            ImGui::PushFont(Fonts::Main());
            ImGui::TableNextColumn();
            ImGui::Text("%s", MASS_TEXT.c_str());
            ImGui::PopFont();

            ImGui::PushFont(Fonts::Data());
            ImGui::TableNextColumn();
            ImGui::Text("%.2e %s", body.GetMass(), "kg");
            ImGui::PopFont();
        }

        auto AddBodyDataRadius(const Body &body) -> void {
            ImGui::PushFont(Fonts::Main());
            ImGui::TableNextColumn();
            ImGui::Text("%s", RADIUS_TEXT.c_str());
            ImGui::PopFont();

            ImGui::PushFont(Fonts::Data());
            ImGui::TableNextColumn();
            ImGui::Text("%.2e %s", body.GetRadius(), "m");
            ImGui::PopFont();
        }

        auto AddBodyDataColor(const Body &body) -> void {
            ImGui::PushFont(Fonts::Main());
            ImGui::TableNextColumn();
            ImGui::Text("%s", COLOR_TEXT.c_str());
            ImGui::PopFont();

            ImGui::PushFont(Fonts::Data());
            ImGui::TableNextColumn();
            ImVec4 color = ImVec4(body.GetColor().r, body.GetColor().g, body.GetColor().b, 1.0F);
            ImGui::ColorEdit3("Body Colour", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
            ImGui::PopFont();
        }

        auto AddBodyDataSpeed(const Body &body) -> void {
            ImGui::PushFont(Fonts::Main());
            ImGui::TableNextColumn();
            ImGui::Text("%s", SPEED_TEXT.c_str());
            ImGui::PopFont();

            ImGui::PushFont(Fonts::Data());
            ImGui::TableNextColumn();
            ImGui::Text("%.2e %s", glm::length(body.GetVelocity()), "m/s");
            ImGui::PopFont();
        }

        auto AddBodyDataAcceleration(const Body &body) -> void {
            ImGui::PushFont(Fonts::Main());
            ImGui::TableNextColumn();
            ImGui::Text("%s", ACCELERATION_TEXT.c_str());
            ImGui::PopFont();

            ImGui::PushFont(Fonts::Data());
            ImGui::TableNextColumn();
            ImGui::Text("%.2e %s", glm::length(Simulation::CalculateAcceleration(Bodies::GetMassiveBodies(), body.GetId(), body.GetPosition())), "m/s\u00B2");
            ImGui::PopFont();
        }

       auto AddBodyDataKineticEnergy(const Body &body) -> void {
           ImGui::PushFont(Fonts::Main());
           ImGui::TableNextColumn();
           ImGui::Text("%s", KINETIC_ENERGY_TEXT.c_str());
           ImGui::PopFont();

           ImGui::PushFont(Fonts::Data());
           ImGui::TableNextColumn();
           ImGui::Text("%.2e %s", Simulation::GetKineticEnergy(body), "J");
           ImGui::PopFont();
       }

       auto AddBodyDataPotentialEnergy(const Body &body) -> void {
           ImGui::PushFont(Fonts::Main());
           ImGui::TableNextColumn();
           ImGui::Text("%s", POTENTIAL_ENERGY_TEXT.c_str());
           ImGui::PopFont();

           ImGui::PushFont(Fonts::Data());
           ImGui::TableNextColumn();
           ImGui::Text("%.2e %s", Simulation::GetPotentialEnergy(body), "J");
           ImGui::PopFont();
       }

       auto AddBodyDataTotalEnergy(const Body &body) -> void {
           ImGui::PushFont(Fonts::Main());
           ImGui::TableNextColumn();
           ImGui::Text("%s", TOTAL_ENERGY_TEXT.c_str());
           ImGui::PopFont();

           ImGui::PushFont(Fonts::Data());
           ImGui::TableNextColumn();
           ImGui::Text("%.2e %s", Simulation::GetKineticEnergy(body) + Simulation::GetPotentialEnergy(body), "J");
           ImGui::PopFont();
       }

        auto AddBodyData() -> void {
            ImGui::Separator();

            // If no body is selected, we don't need to add anything for this section
            if (Bodies::GetSelectedType() == BODY_TYPE_NONE) {
                return;
            }

            Body selectedBody = Bodies::GetBody(Bodies::GetSelectedBody());

            // Title text
            ImGui::PushFont(Fonts::MainBig());
            ImGui::Text("%s", Fonts::NormalizeString(Fonts::MainBig(), selectedBody.GetName(), BODY_DATA_SIZE.x).c_str());
            ImGui::PopFont();

            // Begin
            ImGui::BeginTable("scenario-explorer", 2, BODY_DATA_FLAGS, BODY_DATA_SIZE);

            ImGui::TableSetupColumn("", 0, DATA_KEY_COLUMN_WIDTH);
            ImGui::TableSetupColumn("", 0, DATA_VALUE_COLUMN_WIDTH);

            AddBodyDataMass(selectedBody);
            AddBodyDataRadius(selectedBody);
            AddBodyDataColor(selectedBody);
            AddBodyDataSpeed(selectedBody);
            AddBodyDataAcceleration(selectedBody);
            AddBodyDataKineticEnergy(selectedBody);
            AddBodyDataPotentialEnergy(selectedBody);
            AddBodyDataTotalEnergy(selectedBody);

            // End
            ImGui::EndTable();
        }
    }

    auto Update() -> void {
        // Begin window
        ImGui::Begin("Scenario Explorer", &windowOpen, WINDOW_FLAGS);
        ImGui::SetWindowSize(WINDOW_SIZE);
        ImGui::SetWindowPos(WINDOW_POSITION);

        // Add window contents
        AddExplorer();
        AddBodyData();

        // End window
        ImGui::End();
    }
}