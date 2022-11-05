#include "Style.h"

#include <imgui.h>



namespace Style {
    auto Init() -> void {
        ImGuiStyle& style = ImGui::GetStyle();

        style.Colors[ImGuiCol_Button] = ImVec4(0.0, 0.0, 0.0, 0.0);
        style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.3, 0.4, 0.5, 0.4);
    }
}