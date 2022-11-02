#include "Explorer.h"

#include <algorithm>
#include <ios>
#include <iostream>
#include <main/Bodies.h>
#include <depend/IconsMaterialDesignIcons_c.h>

#include <imgui.h>
#include <string>
#include <unordered_map>
#include <utility>



namespace ScenarioExplorer {
    namespace {
        ImFont* mainFont;
        ImFont* dataFont;
        ImFont* iconFont;

        const string MAIN_FONT_PATH = "../resources/fonts/inter-light.ttf";
        const string DATA_FONT_PATH = "../resources/fonts/source-code-pro-medium.otf";
        const string ICON_FONT_PATH = "../resources/fonts/material-design-icons.ttf";

        const int NAME_WIDTH = 150;
        const int MASS_WIDTH = 155;
        const float FONT_SIZE = 20.0F;

        const unsigned int NAME_COLUMN_ID = 0;
        const unsigned int MASS_COLUMN_ID = 1;

        const ImVec2 WINDOW_SIZE = ImVec2(320, 1000);
        const ImVec2 EXPLORER_SIZE = ImVec2(300, 600);
        
        const ImWchar ICON_RANGE[] = {ICON_MIN_MDI, ICON_MAX_MDI, 0}; // NOLINT (interfacing with C library)

        const ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        const ImGuiTableFlags EXPLORER_FLAGS = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX;

        ImGuiTableSortSpecs* sortSpecs;
        
        bool windowOpen = true;

        auto LoadFonts() -> void {
            ImGuiIO& io = ImGui::GetIO();
            ImFontConfig config;
            config.MergeMode = true;
            config.GlyphMinAdvanceX = FONT_SIZE;

            mainFont = io.Fonts->AddFontFromFileTTF(MAIN_FONT_PATH.c_str(), FONT_SIZE);
            io.Fonts->AddFontFromFileTTF(ICON_FONT_PATH.c_str(), FONT_SIZE, &config, (const ImWchar*)ICON_RANGE);

            dataFont = io.Fonts->AddFontFromFileTTF(DATA_FONT_PATH.c_str(), FONT_SIZE);
            io.Fonts->AddFontFromFileTTF(ICON_FONT_PATH.c_str(), FONT_SIZE, &config, (const ImWchar*)ICON_RANGE);
        }

        auto AddHeader() -> void {
            ImGui::PushFont(mainFont);

            string nameColumn = ICON_MDI_FORMAT_TEXT + string(" Name");
            string massColumn = ICON_MDI_WEIGHT + string(" Mass");
            string speedColumn = ICON_MDI_SPEEDOMETER_SLOW + string(" Speed");

            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn(nameColumn.c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, NAME_WIDTH, NAME_COLUMN_ID);
            ImGui::TableSetupColumn(massColumn.c_str(), ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_PreferSortDescending, MASS_WIDTH, MASS_COLUMN_ID);
            ImGui::TableHeadersRow();

            ImGui::PopFont();
        }

        auto AddAllBodies(vector<string> &bodyIds) -> void {
            for (const auto &pair : Bodies::GetMassiveBodies())  { bodyIds.push_back(pair.first); }
            for (const auto &pair : Bodies::GetMasslessBodies()) { bodyIds.push_back(pair.first); }
        }

        auto AddNameSelectable(const string &id, const string &name) -> void {
            ImGui::TableNextColumn();
            if (ImGui::Selectable(name.c_str(), id == Bodies::GetSelectedBody(), ImGuiSelectableFlags_SpanAllColumns)) {
                Bodies::SetSelectedBody(id);
            }
        }

        auto AddMassiveMassText(const double mass) -> void {
            ImGui::TableNextColumn();
            ImGui::Text("%.2e", mass);
        }

        auto AddMasslessMassText() -> void {
            ImGui::TableNextColumn();
            ImGui::Text("%s", "--------");
        }

        auto AddBodiesToTable(const vector<string> &bodyIds) -> void {
            ImGui::PushFont(dataFont);
            for (const string id : bodyIds) {
                Body body = Bodies::GetBody(id);
                AddNameSelectable(id, body.GetName());
                if (body.GetMass() == 0) {
                    AddMasslessMassText();
                } else {
                    AddMassiveMassText(body.GetMass());
                }
            }
            ImGui::PopFont();
        }

        auto CompareBodyNames(string id1, string id2) -> bool {           
            for (char &c : id1) { c = toupper(c); }
            for (char &c : id2) { c = toupper(c); }

            bool comparison = id1.compare(id2);

            if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                return comparison;
            } else {
                return !comparison;
            }
        }

        auto CompareBodyMasses(const string &id1, const string &id2) -> bool {
            bool comparison = Bodies::GetBody(id1).GetMass() > Bodies::GetBody(id2).GetMass();
            if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                return comparison;
            } else {
                return !comparison;
            }
        }

        auto CompareBodies(const string &id1, const string &id2) -> bool {
            if (sortSpecs->Specs->ColumnIndex == NAME_COLUMN_ID) {
                return CompareBodyNames(id1, id2);
            } else {
                return CompareBodyMasses(id1, id2);
            }
        }

        auto AddTable() -> void {
            ImGui::Begin("Scenario Explorer", &windowOpen, WINDOW_FLAGS);
            ImGui::SetWindowSize(WINDOW_SIZE);
            ImGui::BeginTable("scenario-explorer", 2, EXPLORER_FLAGS, EXPLORER_SIZE);

            ImGui::ShowDemoWindow();

            AddHeader();

            sortSpecs = ImGui::TableGetSortSpecs();
            vector<string> bodyIds;
            AddAllBodies(bodyIds);
            std::sort(bodyIds.begin(), bodyIds.end(), CompareBodies);
            AddBodiesToTable(bodyIds);

            ImGui::EndTable();
            ImGui::End();
        }
    }

    auto Init() -> void {
        LoadFonts();
    }

    auto Update() -> void {
        AddTable();
    }
}