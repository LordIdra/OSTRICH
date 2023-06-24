#pragma once

#include <simulation/SimulationState.h>
#include <bodies/Body.h>



namespace Simulation {

    auto Init() -> void;
    auto PreReset() -> void;
    auto NewBodyReset() -> void;
    auto FrameUpdate() -> void;
    auto Update(const double deltaTime) -> void;
    auto TerminateUpdate() -> void;
    
    auto GetSpeedValue() -> double;
    auto GetSpeedDegree() -> double;
    auto GetMaxSpeedDegree() -> unsigned int;
    auto GetTimeStepSize() -> unsigned int;

    auto GetAcceleration(const string &id) -> dvec3;

    auto GetTimeStep() -> double;
    auto SetTimeStep(const double _timeStep) -> void;
}