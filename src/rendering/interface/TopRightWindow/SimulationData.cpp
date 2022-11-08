#include "SimulationData.h"
#include "depend/IconsMaterialDesignIcons_c.h"
#include "main/Bodies.h"
#include "main/Simulation.h"
#include "rendering/interface/Fonts.h"

#include <imgui.h>
#include <depend/implot/implot.h>
#include <math.h>

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

        const ImPlotAxisFlags AXIS_FLAGS = ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoSideSwitch | ImPlotAxisFlags_NoHighlight;

        const string RESET_TEXT = ICON_MDI_REFRESH + string(" Reset Data");

        auto AddEnergyDeviation() -> void {
            if (ImPlot::BeginPlot("Energy Deviation", ImVec2(385, 100))) {

                // Axes
                ImPlot::SetupAxes(NULL, NULL, AXIS_FLAGS, AXIS_FLAGS);

                // Error
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1.0, 0.0, 0.0, 1.0));
                ImPlot::PlotLine("", &timeValues[0], &(energyDeviation[0]), timeValues.size());
                ImPlot::PopStyleColor();

                ImPlot::EndPlot();
            }
        }

        auto AddSimulationEnergy() -> void {
            if (ImPlot::BeginPlot("Simulation Energy", ImVec2(385, 200))) {

                // Axes
                ImPlot::SetupAxes(NULL, NULL, AXIS_FLAGS, AXIS_FLAGS);

                // Kinetic
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1.0, 0.5, 0.5, 1.0));
                ImPlot::PlotLine("Kinetic", &timeValues[0], &simulationEnergyKinetic[0], timeValues.size());
                ImPlot::PopStyleColor();

                // Potential
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.5, 0.5, 1.0, 1.0));
                ImPlot::PlotLine("Potential", &timeValues[0], &simulationEnergyPotential[0], timeValues.size());
                ImPlot::PopStyleColor();

                // Total
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.5, 1.0, 0.5, 1.0));
                ImPlot::PlotLine("Total", &timeValues[0], &simulationEnergyTotal[0], timeValues.size());
                ImPlot::PopStyleColor();

                ImPlot::EndPlot();
            }
        }

        auto AddBodyEnergy() -> void {
            if (ImPlot::BeginPlot("Body Energy", ImVec2(385, 200))) {

                // Axes
                ImPlot::SetupAxes(NULL, NULL, AXIS_FLAGS, AXIS_FLAGS);

                // Kinetic
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1.0, 0.5, 0.5, 1.0));
                ImPlot::PlotLine("Kinetic", &timeValues[0], &(bodyEnergyKinetic.at(Bodies::GetSelectedBody())[0]), timeValues.size());
                ImPlot::PopStyleColor();

                // Potential
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.5, 0.5, 1.0, 1.0));
                ImPlot::PlotLine("Potential", &timeValues[0], &(bodyEnergyPotential.at(Bodies::GetSelectedBody())[0]), timeValues.size());
                ImPlot::PopStyleColor();

                // Total
                ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.5, 1.0, 0.5, 1.0));
                ImPlot::PlotLine("Total", &timeValues[0], &(bodyEnergyTotal.at(Bodies::GetSelectedBody())[0]), timeValues.size());
                ImPlot::PopStyleColor();

                ImPlot::EndPlot();
            }
        }

        auto AddResetButton() -> void {
            ImGui::PushFont(Fonts::Main());
            if (ImGui::Button(RESET_TEXT.c_str())) {
                Reset();
            }
            ImGui::PopFont();
        }

        auto UpdateEnergyDeviation() -> void {
            energyDeviation.push_back(100 * (Simulation::GetSimulationTotalEnergy() - originalEnergy) / originalEnergy);
        }

        auto UpdateSimulationEnergy() -> void {
            simulationEnergyKinetic.push_back(Simulation::GetSimulationKineticEnergy());
            simulationEnergyPotential.push_back(Simulation::GetSimulationPotentialEnergy());
            simulationEnergyTotal.push_back(Simulation::GetSimulationTotalEnergy());
        }

        auto UpdateBodyEnergy(const std::pair<string, Body> pair) -> void {
            if (bodyEnergyKinetic.count(pair.first) == 0) {
                bodyEnergyKinetic.insert(std::make_pair(pair.first, vector<double>()));
            }

            if (bodyEnergyPotential.count(pair.first) == 0) {
                bodyEnergyPotential.insert(std::make_pair(pair.first, vector<double>()));
            }

            if (bodyEnergyTotal.count(pair.first) == 0) {
                bodyEnergyTotal.insert(std::make_pair(pair.first, vector<double>()));
            }

            bodyEnergyKinetic.at(pair.first).push_back(Simulation::GetKineticEnergy(pair.second));
            bodyEnergyPotential.at(pair.first).push_back(Simulation::GetPotentialEnergy(pair.second));
            bodyEnergyTotal.at(pair.first).push_back(Simulation::GetTotalEnergy(pair.second));
        }
    }

    auto Reset() -> void {
        // Original energy
        originalEnergy = Simulation::GetSimulationTotalEnergy();

        // Time values
        timeValues.clear();

        // Energy deviation
        energyDeviation.clear();

        // Simulation energy
        simulationEnergyKinetic.clear();
        simulationEnergyPotential.clear();
        simulationEnergyTotal.clear();

        // Body energy
        for (const auto &pair : Bodies::GetMassiveBodies()) {
            bodyEnergyKinetic.at(pair.first).clear();
            bodyEnergyPotential.at(pair.first).clear();
            bodyEnergyTotal.at(pair.first).clear();
        }
        for (const auto &pair : Bodies::GetMasslessBodies()) {
            bodyEnergyKinetic.at(pair.first).clear();
            bodyEnergyPotential.at(pair.first).clear();
            bodyEnergyTotal.at(pair.first).clear();
        }
    }

    auto Draw(const double deltaTime) -> void {
        ImPlot::ShowDemoWindow();

        // Set original energy if it hasn't been set
        if (originalEnergy == -1) {
            originalEnergy = Simulation::GetSimulationTotalEnergy();
        }

        // Update values
        timeValues.push_back(Simulation::GetTimeStep());
        UpdateEnergyDeviation();
        UpdateSimulationEnergy();
        for (const auto &pair : Bodies::GetMassiveBodies()) {
            UpdateBodyEnergy(pair);
        }
        for (const auto &pair : Bodies::GetMasslessBodies()) {
            UpdateBodyEnergy(pair);
        }

        // Render graphs
        ImGui::PushFont(Fonts::Data());
        AddEnergyDeviation();
        AddSimulationEnergy();
        if (Bodies::GetSelectedType() != BODY_TYPE_NONE) { AddBodyEnergy(); }
        AddResetButton();
        ImGui::PopFont();
    }
}