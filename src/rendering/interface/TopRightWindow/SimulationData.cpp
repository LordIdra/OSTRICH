#include "SimulationData.h"

#include "depend/IconsMaterialDesignIcons_c.h"
#include "main/Bodies.h"
#include "simulation/Simulation.h"
#include "rendering/interface/Fonts.h"
#include "simulation/SimulationEnergy.h"

#include <imgui.h>
#include <depend/implot/implot.h>

#include <string>
#include <unordered_map>
#include <util/Log.h>



namespace SimulationData {
    namespace {
        double originalEnergy = -1;

        vector<double> timeValues;

        vector<double> energyDeviation;

        vector<double> simulationEnergyKinetic;
        vector<double> simulationEnergyPotential;
        vector<double> simulationEnergyTotal;

        unordered_map<string, vector<double>> bodyEnergyKinetic;
        unordered_map<string, vector<double>> bodyEnergyPotential;
        unordered_map<string, vector<double>> bodyEnergyTotal;

        double timeSinceLastUpdate = 0;

        const ImVec2 ENERGY_DEVIATION_PLOT_SIZE = ImVec2(385, 100);
        const ImVec2 TOTAL_ENERGY_PLOT_SIZE = ImVec2(385, 200);
        const ImVec2 BODY_ENERGY_PLOT_SIZE = ImVec2(385, 200);

        const ImVec4 ENERGY_DEVIATION_LINE_COLOR = ImVec4(1.0, 0.0, 0.0, 1.0);
        const ImVec4 KINETIC_ENERGY_LINE_COLOR = ImVec4(1.0, 0.5, 0.5, 1.0);
        const ImVec4 POTENTIAL_ENERGY_LINE_COLOR = ImVec4(0.5, 0.5, 1.0, 1.0);
        const ImVec4 TOTAL_ENERGY_LINE_COLOR = ImVec4(0.5, 1.0, 0.5, 1.0);

        const ImPlotAxisFlags AXIS_FLAGS = ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoSideSwitch | ImPlotAxisFlags_NoHighlight;

        const string RESET_TEXT = ICON_MDI_REFRESH + string(" Reset Data");

        const double UPDATE_INTERVAL = 0.1;
        const unsigned int MAX_DATA_POINTS = 100;

        auto AddEnergyDeviation() -> void {
            ZoneScoped;
            if (ImPlot::BeginPlot("Energy Deviation", ENERGY_DEVIATION_PLOT_SIZE)) {

                // Axes
                ImPlot::SetupAxes(nullptr, nullptr, AXIS_FLAGS, AXIS_FLAGS);

                // Error
                ImPlot::PushStyleColor(ImPlotCol_Line, ENERGY_DEVIATION_LINE_COLOR);
                ImPlot::PlotLine("", &timeValues[0], &(energyDeviation[0]), timeValues.size());
                ImPlot::PopStyleColor();

                ImPlot::EndPlot();
            }
        }

        auto AddSimulationEnergy() -> void {
            ZoneScoped;
            if (ImPlot::BeginPlot("Simulation Energy", TOTAL_ENERGY_PLOT_SIZE)) {

                // Axes
                ImPlot::SetupAxes(nullptr, nullptr, AXIS_FLAGS, AXIS_FLAGS);

                // Kinetic
                ImPlot::PushStyleColor(ImPlotCol_Line, KINETIC_ENERGY_LINE_COLOR);
                ImPlot::PlotLine("Kinetic", &timeValues[0], &simulationEnergyKinetic[0], timeValues.size());
                ImPlot::PopStyleColor();

                // Potential
                ImPlot::PushStyleColor(ImPlotCol_Line, POTENTIAL_ENERGY_LINE_COLOR);
                ImPlot::PlotLine("Potential", &timeValues[0], &simulationEnergyPotential[0], timeValues.size());
                ImPlot::PopStyleColor();

                // Total
                ImPlot::PushStyleColor(ImPlotCol_Line, TOTAL_ENERGY_LINE_COLOR);
                ImPlot::PlotLine("Total", &timeValues[0], &simulationEnergyTotal[0], timeValues.size());
                ImPlot::PopStyleColor();

                ImPlot::EndPlot();
            }
        }

        auto AddBodyEnergy() -> void {
            ZoneScoped;
            if (ImPlot::BeginPlot("Body Energy", BODY_ENERGY_PLOT_SIZE)) {

                // Axes
                ImPlot::SetupAxes(nullptr, nullptr, AXIS_FLAGS, AXIS_FLAGS);

                // Kinetic
                ImPlot::PushStyleColor(ImPlotCol_Line, KINETIC_ENERGY_LINE_COLOR);
                ImPlot::PlotLine("Kinetic", &timeValues[0], &(bodyEnergyKinetic.at(Bodies::GetSelectedBodyId())[0]), timeValues.size());
                ImPlot::PopStyleColor();

                // Potential
                ImPlot::PushStyleColor(ImPlotCol_Line, POTENTIAL_ENERGY_LINE_COLOR);
                ImPlot::PlotLine("Potential", &timeValues[0], &(bodyEnergyPotential.at(Bodies::GetSelectedBodyId())[0]), timeValues.size());
                ImPlot::PopStyleColor();

                // Total
                ImPlot::PushStyleColor(ImPlotCol_Line, TOTAL_ENERGY_LINE_COLOR);
                ImPlot::PlotLine("Total", &timeValues[0], &(bodyEnergyTotal.at(Bodies::GetSelectedBodyId())[0]), timeValues.size());
                ImPlot::PopStyleColor();

                ImPlot::EndPlot();
            }
        }

        auto AddResetButton() -> void {
            ImGui::PushFont(Fonts::Main());
            if (ImGui::Button(RESET_TEXT.c_str())) {
                PreReset();
            }
            ImGui::PopFont();
        }

        auto UpdateEnergyDeviation() -> void {
            ZoneScoped;
            energyDeviation.push_back(100 * (SimulationEnergy::GetSimulationTotalEnergy() - originalEnergy) / originalEnergy); //NOLINT(cppcoreguidelines-avoid-magic-numbers)
        }

        auto UpdateSimulationEnergy() -> void {
            ZoneScoped;
            simulationEnergyKinetic.push_back(SimulationEnergy::GetSimulationKineticEnergy());
            simulationEnergyPotential.push_back(SimulationEnergy::GetSimulationPotentialEnergy());
            simulationEnergyTotal.push_back(SimulationEnergy::GetSimulationTotalEnergy());
        }

        auto UpdateBodyEnergy(const std::pair<string, Body> &pair) -> void {
            ZoneScoped;
            bodyEnergyKinetic.at(pair.first).push_back(SimulationEnergy::GetKineticEnergy(pair.second));
            bodyEnergyPotential.at(pair.first).push_back(SimulationEnergy::GetPotentialEnergy(pair.second));
            bodyEnergyTotal.at(pair.first).push_back(SimulationEnergy::GetTotalEnergy(pair.second));
        }

        auto RemoveFirstElement(vector<double> &data) -> void {
            data.erase(data.begin());
        }

        auto ClipData() -> void {
            if (timeValues.size() > MAX_DATA_POINTS) {
                RemoveFirstElement(timeValues);
                RemoveFirstElement(energyDeviation);
                RemoveFirstElement(simulationEnergyKinetic);
                RemoveFirstElement(simulationEnergyPotential);
                RemoveFirstElement(simulationEnergyTotal);

                for (auto &pair : bodyEnergyKinetic)   { RemoveFirstElement(pair.second); }
                for (auto &pair : bodyEnergyPotential) { RemoveFirstElement(pair.second); }
                for (auto &pair : bodyEnergyTotal)     { RemoveFirstElement(pair.second); }
            }
        }
    }

    auto PreReset() -> void {
        timeValues.clear();
        energyDeviation.clear();
        simulationEnergyKinetic.clear();
        simulationEnergyPotential.clear();
        simulationEnergyTotal.clear();
        bodyEnergyKinetic.clear();
        bodyEnergyPotential.clear();
        bodyEnergyTotal.clear();
    }

    auto PostReset() -> void {
        originalEnergy = SimulationEnergy::GetSimulationTotalEnergy();
    }

    auto NewBodyReset() -> void {
        for (const string &id : Bodies::GetBodyIds()) { 
            bodyEnergyKinetic.insert(std::make_pair(id, vector<double>()));\
            bodyEnergyPotential.insert(std::make_pair(id, vector<double>()));\
            bodyEnergyTotal.insert(std::make_pair(id, vector<double>()));\
        }
    }

    auto Draw(const double deltaTime) -> void {
        // Update time
        timeSinceLastUpdate += deltaTime;

        // Set original energy if it hasn't been set
        if (originalEnergy == -1) {
            originalEnergy = SimulationEnergy::GetSimulationTotalEnergy();
        }

        // Update values if necessary
        if (timeSinceLastUpdate > UPDATE_INTERVAL) {
            timeSinceLastUpdate = 0;
            timeValues.push_back(Simulation::GetTimeStep());
            UpdateEnergyDeviation();
            UpdateSimulationEnergy();
            for (const auto &pair : Bodies::GetBodies()) {
                UpdateBodyEnergy(pair);
            }
            ClipData();
        }

        // Render graphs
        ImGui::PushFont(Fonts::Data());
        AddEnergyDeviation();
        AddSimulationEnergy();
        if (Bodies::IsBodySelected()) { AddBodyEnergy(); }
        AddResetButton();
        ImGui::PopFont();
    }
}