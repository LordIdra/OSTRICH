#include "LoadScenario.h"

#include <rendering/interface/Fonts.h>

#include <util/Types.h>
#include <depend/IconsMaterialDesignIcons_c.h>
#include <imgui.h>



namespace LoadScenario {
    namespace {
        const string TITLE_TEXT = ICON_MDI_FOLDER_DOWNLOAD + string(" Load Scenario");
        const string NAME_TEXT = ICON_MDI_FORMAT_TEXT + string(" Name");
        const string BODIES_TEXT = ICON_MDI_EARTH + string(" Bodies");
        const string TIME_TEXT = ICON_MDI_CLOCK + string(" Time");

        const string SAVE_TEXT = ICON_MDI_CHECK_CIRCLE_OUTLINE + string(" Load");
        const string CANCEL_TEXT = ICON_MDI_CLOSE_CIRCLE_OUTLINE + string(" Cancel");

        const int NAME_WIDTH = 300;
        const int BODIES_WEIGHT = 50;
        const int TIME_WEIGHT = 80;

        const unsigned int NAME_COLUMN_ID = 0;
        const unsigned int BODIES_COLUMN_ID = 1;
        const unsigned int TIME_COLUMN_ID = 2;

        const ImVec2 TABLE_SIZE = ImVec2(600, 400);

        const ImGuiPopupFlags POPUP_FLAGS = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;
        const ImGuiTableFlags TABLE_FLAGS = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX;

        string selectedFile;

        auto AddTitle() -> void {
            ImGui::PushFont(Fonts::MainBig());
            ImGui::Text("%s", Fonts::NormalizeString(Fonts::MainBig(), TITLE_TEXT, TABLE_SIZE.x).c_str());
            ImGui::PopFont();
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

        auto AddTimeText(const string text) -> void {
            ImGui::TableNextColumn();
            ImGui::Text("%s", text.c_str());
        }

        auto AddFileTable() -> void {
            if (ImGui::BeginTable("file-table", 3, TABLE_FLAGS, TABLE_SIZE)) {
                AddHeader();

                ImGui::PushFont(Fonts::Data());
                AddNameText("hello mortals");
                AddBodiesText(666);
                AddTimeText("5y 78d 20h 3m 8s");
                ImGui::PopFont();

                ImGui::EndTable();
            }
        }

        auto AddButtons() -> void {
            if (ImGui::Button(SAVE_TEXT.c_str(), ImVec2(120, 0))) { 
                ImGui::CloseCurrentPopup(); 
            }

            ImGui::SameLine();

            if (ImGui::Button(CANCEL_TEXT.c_str(), ImVec2(120, 0))) { 
                ImGui::CloseCurrentPopup(); 
            }
        }
    }

    auto Draw() -> void {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1, 0.1, 0.1, 0.7));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1, 0.1, 0.1, 0.7));

        if (ImGui::BeginPopupModal("Load Scenario", NULL, POPUP_FLAGS)) {
            AddTitle();
            AddFileTable();
            AddButtons();
            ImGui::EndPopup();
        }

        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
}