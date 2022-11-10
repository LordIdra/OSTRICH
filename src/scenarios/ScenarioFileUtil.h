#pragma once

#include <util/Types.h>
#include <rendering/structures/Material.h>

#include <yaml-cpp/emitter.h>



struct ScenarioFile {
    string nameWithoutExtension;
    int bodyCount;
    int rawTime;
    string formattedTime;
};

namespace ScenarioFileUtil {

    auto GetOnlyFilename(const string &path) -> string;
    auto SaveFile(const YAML::Emitter &scenario, const string &path) -> void;
    auto AddPrefixAndSuffix(const string &path) -> string;

    auto GenerateMaterial(const vec3 color) -> Material;

    auto GetBodyCount(const YAML::Node &scenario) -> int;
    auto GetTime(const YAML::Node &scenario) -> int;

    auto GetScenarioFile(const YAML::Node &scenario, const string &path) -> ScenarioFile;
    auto ScenarioExists(const string &scenarioPath) -> bool;
    auto GetScenarios() -> vector<ScenarioFile>;
    
}