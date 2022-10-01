#pragma once

#include <bodies/Massive.h>
#include <unordered_map>



namespace Simulation {

    auto Init() -> void;

    auto Integrate(const unordered_map<string, Massive> &massive_bodies, Body &body) -> void;
    auto Integrate(unordered_map<string, Massive> &massive_bodies) -> void;

    auto SetTimeStepSize(double size) -> void;
    auto SetTimeStepsPerFrame(double size) -> void;
}