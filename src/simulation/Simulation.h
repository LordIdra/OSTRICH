#pragma once

#include <simulation/SimulationState.h>


namespace Simulation {

    auto Init() -> void;
    auto PreReset() -> void;
    auto NewBodyReset() -> void;
    auto Update(const double deltaTime) -> void;

    auto GetFutureStates() -> vector<SimulationState>;
    auto GetPastStates() -> vector<SimulationState>;
    
    auto GetSpeedValue() -> double;
    auto GetSpeedDegree() -> double;
    auto GetMaxSpeedDegree() -> unsigned int;
    auto GetTimeStepSize() -> unsigned int;

    auto GetTimeStep() -> double;
    auto SetTimeStep(const double _timeStep) -> void;
}