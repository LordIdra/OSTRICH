#pragma once

#include "main/OrbitPoint.h"
#include <bodies/Massive.h>
#include <bodies/Massless.h>
#include <unordered_map>



namespace Simulation {

    auto Init() -> void;

    auto Integrate(const unordered_map<string, Massive> &massiveBodies, const string &id, const double time_step, const OrbitPoint &point) -> OrbitPoint;
    auto Integrate(const unordered_map<string, Massive> &massive_bodies, Body &body) -> void;
    auto Integrate(unordered_map<string, Massive> massiveBodies, unordered_map<string, Massless> masslessBodies) -> unordered_map<string, vector<OrbitPoint>>;

    auto SetTimeStepSize(const double size) -> void;
    auto SetTimeStepsPerFrame(const double size) -> void;
}