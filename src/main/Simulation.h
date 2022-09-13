#pragma once

#include <bodies/Massive.h>
#include <unordered_map>



namespace Simulation {

    auto Integrate(const unordered_map<string, Massive> &massive_bodies, Body &body) -> void;
    auto Integrate(unordered_map<string, Massive> &massive_bodies) -> void;

    auto SetTimeStepSize(bfloat size) -> void;
    auto SetTimeStepsPerFrame(bfloat size) -> void;
}