#include "Scenarios.h"

#include <scenarios/YMLUtil.h>
#include <scenarios/ScenarioFileUtil.h>
#include <bodies/Massive.h>
#include <simulation/Simulation.h>
#include <rendering/shaders/Util.h>
#include <main/Control.h>
#include <util/Log.h>
#include <main/Bodies.h>
#include <rendering/interface/TopRightWindow/SimulationData.h>

#include <yaml-cpp/yaml.h>

#include <string>
#include <chrono>
#include <filesystem>



namespace Scenarios {

    namespace {
        auto ScenarioContainsRequiredKeys(const YAML::Node &scenario) -> bool {
            return scenario["time"]  && scenario["massive"]  && scenario["massless"];
        }

        auto LoadMassive(const string &id, const YAML::Node &node) -> Massive {
            string name =   YMLUtil::GetString(node, "name");
            vec3 color =    YMLUtil::GetVec3  (node, "color");
            double radius = YMLUtil::GetDouble(node, "radius");
            double mass =   YMLUtil::GetDouble(node, "mass");
            vec3 position = YMLUtil::GetVec3  (node, "position");
            vec3 velocity = YMLUtil::GetVec3  (node, "velocity");
            Material material = ScenarioFileUtil::GenerateMaterial(color);
            return Massive(id, name, color, position, velocity, material, mass, radius);
        }

        auto LoadMassless(const string &id, const YAML::Node &node) -> Massless {
            string name =   YMLUtil::GetString(node, "name");
            vec3 color =    YMLUtil::GetVec3  (node, "color");
            vec3 position = YMLUtil::GetVec3  (node, "position");
            vec3 velocity = YMLUtil::GetVec3  (node, "velocity");
            return Massless(id, name, color, position, velocity);
        }

        auto LoadMassiveBodies(const YAML::Node &scenario) -> void {
            YAML::Node massive = scenario["massive"];
            for (YAML::const_iterator i = massive.begin(); i != massive.end(); i++) {

                // 'massive' node id and 'massive' node body
                auto id = i->first.as<string>();
                YAML::Node node = i->second;

                // Load body
                Bodies::AddBody(LoadMassive(id, node));
            }
        }

        auto LoadMasslessBodies(const YAML::Node &scenario) -> void {
            YAML::Node massless = scenario["massless"];
            for (YAML::const_iterator i = massless.begin(); i != massless.end(); i++) {

                // 'massive' node id and 'massive' node body
                auto id = i->first.as<string>();
                YAML::Node node = i->second;

                // Load body
                Bodies::AddBody(LoadMassless(id, node));
            }
        }
        
        auto LoadTime(const YAML::Node &scenario) -> void {
            int time = YMLUtil::GetInt(scenario, "time");
            Simulation::SetTimeStep(time);
        }

        auto SaveMassive(const string &id, YAML::Emitter &scenario, const Massive &body) -> void {
            scenario << id;
            scenario << YAML::BeginMap;
            YMLUtil::SetString(scenario, "name", body.GetName());
            YMLUtil::SetVec3(scenario, "color", body.GetColor());
            YMLUtil::SetDouble(scenario, "radius", body.GetRadius());
            YMLUtil::SetDouble(scenario, "mass", body.GetMass());
            YMLUtil::SetVec3(scenario, "position", body.GetPosition());
            YMLUtil::SetVec3(scenario, "velocity", body.GetVelocity());
            scenario << YAML::EndMap;
        }

        auto SaveMassless(const string &id, YAML::Emitter &scenario, const Massless &body) -> void {
            scenario << id;
            scenario << YAML::BeginMap;
            YMLUtil::SetString(scenario, "name", body.GetName());
            YMLUtil::SetVec3(scenario, "color", body.GetColor());
            YMLUtil::SetVec3(scenario, "position", body.GetPosition());
            YMLUtil::SetVec3(scenario, "velocity", body.GetVelocity());
            scenario << YAML::EndMap;
        }

        auto SaveMassiveBodies(YAML::Emitter &scenario) -> void {
            scenario << YAML::Key << "massive";
            scenario << YAML::Value << YAML::BeginMap;
            for (const auto &pair : Bodies::GetMassiveBodies()) {
                string id = pair.first;
                Massive body = pair.second;
                SaveMassive(id, scenario, body);
            }
            scenario << YAML::EndMap;
        }

        auto SaveMasslessBodies(YAML::Emitter &scenario) -> void {
            // Save massless bodies
            scenario << YAML::Key << "massless";
            scenario << YAML::Value << YAML::BeginMap;
            for (const auto &pair : Bodies::GetMasslessBodies()) {
                string id = pair.first;
                Massless body = pair.second;
                SaveMassless(id, scenario, body);
            }
            scenario << YAML::EndMap;
        }

        auto SaveTime(YAML::Emitter &scenario) -> void {
            scenario << YAML::Key << "time";
            scenario << YAML::Value << int(Simulation::GetTimeStep());
        }
    }

    auto LoadScenario(const string &filenameWithoutExtension) -> void {
        const string path = ScenarioFileUtil::AddPrefixAndSuffix(filenameWithoutExtension);

        if (!FileExists(path)) {
            YMLUtil::SetCurrentError(YMLUtil::FILE_NOT_FOUND);
            return;
        }

        YAML::Node scenario = YAML::LoadFile(path);

        if (!ScenarioContainsRequiredKeys(scenario)) { 
            YMLUtil::SetCurrentError(YMLUtil::MISSING_BASE_KEY);
            return; 
        }

        Control::PreReset();

        LoadTime(scenario);
        LoadMassiveBodies(scenario);
        LoadMasslessBodies(scenario);

        Control::PostReset();
    }

    auto SaveScenario(const string &filenameWithoutExtension) -> void {
        const string path = ScenarioFileUtil::AddPrefixAndSuffix(filenameWithoutExtension);

        YAML::Emitter scenario;

        scenario << YAML::BeginMap;

        SaveTime(scenario);
        SaveMassiveBodies(scenario);
        SaveMasslessBodies(scenario);

        scenario << YAML::EndMap;

        ScenarioFileUtil::SaveFile(scenario, path);
    }
}