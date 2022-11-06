#pragma once

#include "main/OrbitPoint.h"
#include <bodies/Massive.h>
#include <bodies/Massless.h>
#include <unordered_map>



namespace Simulation {

    const double SPEED_MULTIPLIER = 5;
    const double MIN_MULTIPLIER = 1;
    const double MAX_MULTIPLIER = 12;

    auto Init() -> void;

    auto CalculateAcceleration(const unordered_map<string, Massive> &massiveBodies, const string &id, const dvec3 &position) -> dvec3;

    auto GenerateNextOrbitPoint(const unordered_map<string, Massive> &massiveBodies, const string &id, const double time_step, const OrbitPoint &point) -> OrbitPoint;
    auto UpdateBodyPosition(const unordered_map<string, Massive> &massiveBodies, Body &body) -> void;
    auto RegenerateFuturePoints() -> unordered_map<string, vector<OrbitPoint>>;

    auto GetKineticEnergy(const Body &body) -> double;
    auto GetPotentialEnergy(const Body &body) -> double;

    auto SetTimeStepSize(const double size) -> void;
    auto SetTimeStepsPerFrame(const double size) -> void;

    auto GetTimeStepSize() -> double;
    auto GetSimulationSpeed() -> double;
    auto GetSimulationSpeedMultiplier() -> double;
}