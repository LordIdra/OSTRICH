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

        const double MASS_THRESHOLD = 100000; // Bodies above this mass (in kg) will be considered Massive
        
        string scenarioToLoadNextFrame = "";

        auto ScenarioContainsRequiredKeys(const YAML::Node &scenario) -> bool {
            return scenario["time"]  && scenario["bodies"];
        }

        auto LoadBody(const string &id, const YAML::Node &node) -> void {
            string name =   YMLUtil::GetString(node, "name");
            vec3 color =    YMLUtil::GetVec3  (node, "color");
            double radius = YMLUtil::GetDouble(node, "radius");
            double mass =   YMLUtil::GetDouble(node, "mass");
            vec3 position = YMLUtil::GetVec3  (node, "position");
            vec3 velocity = YMLUtil::GetVec3  (node, "velocity");
            if (mass > MASS_THRESHOLD) {
                Material material = ScenarioFileUtil::GenerateMaterial(color);
                Bodies::AddBody(Massive(id, name, color, position, velocity, mass, radius, material));
            } else {
                Bodies::AddBody(Massless(id, name, color, position, velocity, mass, radius));
            }
        }

        auto LoadBodies(const YAML::Node &scenario) -> void {
            YAML::Node bodies = scenario["bodies"];
            for (YAML::const_iterator i = bodies.begin(); i != bodies.end(); i++) {
                auto id = i->first.as<string>();
                YAML::Node node = i->second;
                LoadBody(id, node);
            }
        }
        
        auto LoadTime(const YAML::Node &scenario) -> void {
            int time = YMLUtil::GetInt(scenario, "time");
            Simulation::SetTimeStep(time);
        }

        auto SaveBody(const string &id, YAML::Emitter &scenario, const Body &body) -> void {
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

        auto SaveBodies(YAML::Emitter &scenario) -> void {
            scenario << YAML::Key << "bodies";
            scenario << YAML::Value << YAML::BeginMap;
            for (const auto &pair : Bodies::GetBodies()) {
                string id = pair.first;
                Body body = pair.second;
                SaveBody(id, scenario, body);
            }
            scenario << YAML::EndMap;
        }

        auto SaveTime(YAML::Emitter &scenario) -> void {
            scenario << YAML::Key << "time";
            scenario << YAML::Value << int(Simulation::GetTimeStep());
        }

        auto LoadScheduledScenario() -> void {
            const string path = ScenarioFileUtil::AddPrefixAndSuffix(scenarioToLoadNextFrame);

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
            LoadBodies(scenario);

            Control::PostReset();
        }
    }

    auto FrameUpdate() -> void {
        if (scenarioToLoadNextFrame != "") {
            LoadScheduledScenario();
            scenarioToLoadNextFrame = "";
        }
    }

    auto ScheduleLoadScenario(const string &filenameWithoutExtension) -> void {
        scenarioToLoadNextFrame = filenameWithoutExtension;
    }

    auto SaveScenario(const string &filenameWithoutExtension) -> void {
        const string path = ScenarioFileUtil::AddPrefixAndSuffix(filenameWithoutExtension);

        YAML::Emitter scenario;

        scenario << YAML::BeginMap;

        SaveTime(scenario);
        SaveBodies(scenario);

        scenario << YAML::EndMap;

        ScenarioFileUtil::SaveFile(scenario, path);
    }
}