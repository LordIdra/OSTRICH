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
    auto Update(const double deltaTime) -> void;
    auto PreReset() -> void;

    auto PostReset() -> void;

    auto CalculateLatestAcceleration(const string &id, const dvec3 &position) -> dvec3;

    auto GenerateNextOrbitPoint(const string &id, const OrbitPoint &point) -> OrbitPoint;
    
    auto GetPastOrbitPoints() -> unordered_map<string, vector<OrbitPoint>>;
    auto GetFutureOrbitPoints() -> unordered_map<string, vector<OrbitPoint>>;

    auto GetKineticEnergy(const Body &body) -> double;
    auto GetPotentialEnergy(const Body &body) -> double;

    auto GetTotalEnergy(const Body &body) -> double;

    auto SetTimeStepSize(const double size) -> void;
    auto SetTimeStep(const double step) -> void;
    auto SetTimeStepsPerFrame(const double size) -> void;

    auto GetTimeStepSize() -> double;
    auto GetTimeStep() -> double;
    auto GetSimulationSpeed() -> double;
    auto GetSimulationSpeedMultiplier() -> double;
    
    auto GetSimulationKineticEnergy() -> double;
    auto GetSimulationPotentialEnergy() -> double;
    auto GetSimulationTotalEnergy() -> double;
}