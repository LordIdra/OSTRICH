#pragma once

#include <simulation/SimulationState.h>



namespace OrbitPaths {
    auto Init() -> void;
    auto NewBodyReset() -> void;
    auto Update() -> void;
    
    auto AddNewState(const SimulationState &state) -> void;
    auto StepToNextState(const SimulationState &state) -> void;

    auto GetMaxFutureStates() -> unsigned int;
}