#pragma once

#include <util/Types.h>




namespace Scenarios {
    struct ScenarioFile {
        string nameWithExtension;
        int bodyCount;
        int rawTime;
        string formattedTime;
    };

    auto SaveScenario(const string &path) -> void;
    auto LoadScenario(const string &pathWithoutExtension) -> void;

    auto ScenarioExists(const string &path) -> bool;
    auto GetScenarios() -> vector<ScenarioFile>;
}