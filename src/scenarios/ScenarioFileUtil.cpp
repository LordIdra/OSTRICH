#include "ScenarioFileUtil.h"
#include "YMLUtil.h"

#include <util/Types.h>
#include <util/TimeFormat.h>

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <filesystem>
#include <fstream>



namespace ScenarioFileUtil {
    namespace {
        const string SCENARIO_DIRECTORY = "../scenarios/";
        const string YML_SUFFIX = ".yml";

        const float AMBIENT = 0.1;
        const float DIFFUSE = 0.8;
        const float SPECULAR = 0.3;
        const float SHINE = 32;
    }

    auto GetOnlyFilename(const string &path) -> string {
        string withoutPrefix = path.substr(SCENARIO_DIRECTORY.size(), path.size() - SCENARIO_DIRECTORY.size());
        string withoutSuffix = withoutPrefix.substr(0, withoutPrefix.size()-4);
        return withoutSuffix;
    }

    auto AddPrefixAndSuffix(const string &path) -> string {
        return SCENARIO_DIRECTORY + path + YML_SUFFIX;
    }

    auto SaveFile(const YAML::Emitter &scenario, const string &path) -> void {
        std::ofstream file(path);
        file << scenario.c_str();
        file.close();
    }

    auto GenerateMaterial(const vec3 color) -> Material {
        return Material{
            .ambient = AMBIENT * color, 
            .diffuse = DIFFUSE * color, 
            .specular = SPECULAR * color,
            .shine = SHINE};
    }

    auto GetBodyCount(const YAML::Node &scenario) -> int {
        int bodyCount = 0;

        YAML::Node bodies = scenario["bodies"];

        for (YAML::const_iterator i = bodies.begin(); i != bodies.end(); i++)   { bodyCount++; }

        return bodyCount;
    }

    auto GetTime(const YAML::Node &scenario) -> int {
        return YMLUtil::GetInt(scenario, "time");
    }

    auto GetScenarioFile(const YAML::Node &scenario, const string &path) -> ScenarioFile {
        string fileName = GetOnlyFilename(path);
        int bodyCount = GetBodyCount(scenario);
        int rawTime = GetTime(scenario);
        string formattedTime = TimeFormat::FormatTime(rawTime);

        return ScenarioFile{fileName, bodyCount, rawTime, formattedTime};
    }

    auto ScenarioExists(const string &scenarioPath) -> bool {
        for (const auto &entry : std::filesystem::directory_iterator(SCENARIO_DIRECTORY)) {
            const string path = entry.path();
            if (path.substr(path.size()-4, 4) != ".yml") {
                continue;
            }
            if (ScenarioFileUtil::GetOnlyFilename(path) == scenarioPath) {
                return true;
            }
        }

        return false;
    }

    auto GetScenarios() -> vector<ScenarioFile> {
        vector<ScenarioFile> scenarios;
        for (const auto &entry : std::filesystem::directory_iterator(SCENARIO_DIRECTORY)) {
            const string path = entry.path();
            if (path.substr(path.size()-4, 4) != ".yml") {
                continue;
            }
            YAML::Node scenario = YAML::LoadFile(entry.path());
            scenarios.emplace_back(GetScenarioFile(scenario, path));
        }
        return scenarios;
    }
}