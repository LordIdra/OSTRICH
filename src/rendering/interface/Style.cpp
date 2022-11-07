#include "Style.h"

#include <imgui.h>
#include <depend/implot/implot.h>



namespace Style {
    auto Init() -> void {
        ImGuiStyle &guiStyle = ImGui::GetStyle();
        ImPlotStyle &plotStyle = ImPlot::GetStyle();

        guiStyle.Colors[ImGuiCol_Button] = ImVec4(0.0, 0.0, 0.0, 0.0);
        guiStyle.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.3, 0.4, 0.5, 0.4);

        plotStyle.Colors[ImPlotCol_FrameBg] = ImVec4(0.0, 0.0, 0.0, 0.0);
        plotStyle.Colors[ImPlotCol_PlotBg] = ImVec4(0.0, 0.0, 0.0, 0.8);
        plotStyle.Colors[ImPlotCol_LegendBorder] = ImVec4(0.0, 0.0, 0.0, 0.0);
        plotStyle.Colors[ImPlotCol_LegendBg] = ImVec4(0.0, 0.0, 0.0, 0.1);
    }
}