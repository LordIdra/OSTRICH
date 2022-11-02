#include "Explorer.h"

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
        const int MASS_WIDTH = 100;
        const int SPEED_WIDTH = 100;

        const float FONT_SIZE = 20.0F;

        const ImVec2 WINDOW_SIZE = ImVec2(410, 1000);
        const ImVec2 EXPLORER_SIZE = ImVec2(400, 600);
        
        const ImWchar ICON_RANGE[] = {ICON_MIN_MDI, ICON_MAX_MDI, 0}; // NOLINT (interfacing with C library)

        const ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        const ImGuiTableFlags EXPLORER_FLAGS = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY;
        const ImGuiTableFlags EXPLORER_COLUMN_FLAGS = ImGuiTableColumnFlags_WidthFixed;
        
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
            ImGui::TableSetupColumn(nameColumn.c_str(), EXPLORER_COLUMN_FLAGS, NAME_WIDTH);
            ImGui::TableSetupColumn(massColumn.c_str(), EXPLORER_COLUMN_FLAGS, MASS_WIDTH);
            ImGui::TableSetupColumn(speedColumn.c_str(), EXPLORER_COLUMN_FLAGS, SPEED_WIDTH);
            ImGui::TableHeadersRow();

            ImGui::PopFont();
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

        auto AddVelocityText(const vec3 velocity) -> void {
            ImGui::TableNextColumn();
            ImGui::Text("%.2e", glm::length(velocity));
        }

        auto AddMassiveBodies() -> void {
            ImGui::PushFont(dataFont);

            for (const auto &pair : Bodies::GetMassiveBodies()) {
                AddNameSelectable(pair.first, pair.second.GetName());
                AddMassiveMassText(pair.second.GetMass());
                AddVelocityText(pair.second.GetVelocity());
            }

            ImGui::PopFont();
        }

        auto AddMasslessBodies() -> void {
            ImGui::PushFont(dataFont);

            for (const auto &pair : Bodies::GetMasslessBodies()) {
                AddNameSelectable(pair.first, pair.second.GetName());
                AddMasslessMassText();
                AddVelocityText(pair.second.GetVelocity());
            }

            ImGui::PopFont();
        }

        auto AddTable() -> void {
            ImGui::Begin("Scenario Explorer", &windowOpen, WINDOW_FLAGS);
            ImGui::SetWindowSize(WINDOW_SIZE);
            ImGui::BeginTable("scenario-explorer", 3, EXPLORER_FLAGS, EXPLORER_SIZE);

            ImGui::ShowDemoWindow();

            AddHeader();
            AddMassiveBodies();
            AddMasslessBodies();

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