#include "BodyData.h"

#include "simulation/Simulation.h"
#include "simulation/SimulationEnergy.h"
#include <main/Bodies.h>
#include <rendering/interface/Fonts.h>

#include <imgui.h>



namespace BodyData {
    namespace {

        const string MASS_TEXT = ICON_MDI_WEIGHT + string(" Mass");
        const string RADIUS_TEXT = ICON_MDI_RADIUS + string(" Radius");
        const string COLOR_TEXT = ICON_MDI_BRUSH + string(" Color");
        const string SPEED_TEXT = ICON_MDI_SPEEDOMETER_SLOW + string(" Velocity");
        const string ACCELERATION_TEXT = ICON_MDI_CHEVRON_DOUBLE_UP + string(" Acceleration");
        const string KINETIC_ENERGY_TEXT = ICON_MDI_CUBE_SEND + string(" Kinetic Energy");
        const string POTENTIAL_ENERGY_TEXT = ICON_MDI_ATOM + string(" Potential Energy");
        const string TOTAL_ENERGY_TEXT = ICON_MDI_LIGHTNING_BOLT + string(" Total Energy");

        const int KEY_COLUMN_WIDTH = 170;
        const int VALUE_COLUMN_WIDTH =  130;

        const ImVec2 TABLE_SIZE = ImVec2(300, 100);
        const ImGuiTableFlags TABLE_FLAGS = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoPadOuterX | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_SizingFixedFit;

        auto AddMass(const Body &body) -> void {
             ImGui::PushFont(Fonts::Main());
             ImGui::TableNextColumn();
             ImGui::Text("%s", MASS_TEXT.c_str());
             ImGui::PopFont();

             ImGui::PushFont(Fonts::Data());
             ImGui::TableNextColumn();
             ImGui::Text("%.2e %s", body.GetMass(), "kg");
             ImGui::PopFont();
         }

         auto AddRadius(const Body &body) -> void {
             ImGui::PushFont(Fonts::Main());
             ImGui::TableNextColumn();
             ImGui::Text("%s", RADIUS_TEXT.c_str());
             ImGui::PopFont();

             ImGui::PushFont(Fonts::Data());
             ImGui::TableNextColumn();
             ImGui::Text("%.2e %s", body.GetRadius(), "m");
             ImGui::PopFont();
         }

         auto AddColor(const Body &body) -> void {
             ImGui::PushFont(Fonts::Main());
             ImGui::TableNextColumn();
             ImGui::Text("%s", COLOR_TEXT.c_str());
             ImGui::PopFont();

             ImGui::PushFont(Fonts::Data());
             ImGui::TableNextColumn();
             ImVec4 color = ImVec4(body.GetColor().r, body.GetColor().g, body.GetColor().b, 1.0F);
             ImGui::ColorEdit3("Body Colour", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); //NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
             ImGui::PopFont();
         }

         auto AddSpeed(const Body &body) -> void {
             ImGui::PushFont(Fonts::Main());
             ImGui::TableNextColumn();
             ImGui::Text("%s", SPEED_TEXT.c_str());
             ImGui::PopFont();

             ImGui::PushFont(Fonts::Data());
             ImGui::TableNextColumn();
             ImGui::Text("%.2e %s", glm::length(body.GetVelocity()), "m/s");
             ImGui::PopFont();
         }

         auto AddAcceleration(const Body &body) -> void {
             ImGui::PushFont(Fonts::Main());
             ImGui::TableNextColumn();
             ImGui::Text("%s", ACCELERATION_TEXT.c_str());
             ImGui::PopFont();

             ImGui::PushFont(Fonts::Data());
             ImGui::TableNextColumn();
             ImGui::Text("%.2e", glm::length(Simulation::GetAcceleration(body.GetId())));
             ImGui::PopFont();
         }

        auto AddKineticEnergy(const Body &body) -> void {
            ImGui::PushFont(Fonts::Main());
            ImGui::TableNextColumn();
            ImGui::Text("%s", KINETIC_ENERGY_TEXT.c_str());
            ImGui::PopFont();

            ImGui::PushFont(Fonts::Data());
            ImGui::TableNextColumn();
            ImGui::Text("%.2e %s", SimulationEnergy::GetKineticEnergy(body), "J");
            ImGui::PopFont();
        }

        auto AddPotentialEnergy(const Body &body) -> void {
            ImGui::PushFont(Fonts::Main());
            ImGui::TableNextColumn();
            ImGui::Text("%s", POTENTIAL_ENERGY_TEXT.c_str());
            ImGui::PopFont();

            ImGui::PushFont(Fonts::Data());
            ImGui::TableNextColumn();
            ImGui::Text("%.2e %s", SimulationEnergy::GetPotentialEnergy(body), "J");
            ImGui::PopFont();
        }

        auto AddTotalEnergy(const Body &body) -> void {
            ImGui::PushFont(Fonts::Main());
            ImGui::TableNextColumn();
            ImGui::Text("%s", TOTAL_ENERGY_TEXT.c_str());
            ImGui::PopFont();

            ImGui::PushFont(Fonts::Data());
            ImGui::TableNextColumn();
            ImGui::Text("%.2e %s", SimulationEnergy::GetKineticEnergy(body) + SimulationEnergy::GetPotentialEnergy(body), "J");
            ImGui::PopFont();
        }
    }

    auto Draw() -> void {
        // If no body is selected, we don't need to add anything for this section
        if (Bodies::GetSelectedType() == BODY_TYPE_NONE) {
            return;
        }

        ImGui::Separator();

        Body selectedBody = Bodies::GetBody(Bodies::GetSelectedBody());

        // Title text
        ImGui::PushFont(Fonts::MainBig());
        ImGui::Text("%s", Fonts::NormalizeString(Fonts::MainBig(), selectedBody.GetName(), TABLE_SIZE.x).c_str());
        ImGui::PopFont();

        // Begin
        ImGui::BeginTable("scenario-explorer", 2, TABLE_FLAGS, TABLE_SIZE);

        ImGui::TableSetupColumn("", 0, KEY_COLUMN_WIDTH);
        ImGui::TableSetupColumn("", 0, VALUE_COLUMN_WIDTH);

        AddMass(selectedBody);
        AddRadius(selectedBody);
        AddColor(selectedBody);
        AddSpeed(selectedBody);
        AddAcceleration(selectedBody);
        AddKineticEnergy(selectedBody);
        AddPotentialEnergy(selectedBody);
        AddTotalEnergy(selectedBody);

        // End
        ImGui::EndTable();
    }
}