#pragma once

#include <util/Types.h>
#include <scenarios/ScenarioFileUtil.h>




namespace Scenarios {
    auto SaveScenario(const string &filenameWithoutExtension) -> void;
    auto FrameUpdate() -> void;
    auto ScheduleLoadScenario(const string &filenameWithoutExtension) -> void;
}