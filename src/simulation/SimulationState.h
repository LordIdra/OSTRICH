#pragma once

#include <main/OrbitPoint.h>
#include <util/Types.h>



class SimulationState {
private:
    unordered_map<string, OrbitPoint> points;

    auto CalculateIndividualAcceleration(const string &accelerationOf, const string &withRespectTo) -> dvec3;
    auto CalculateTotalAcceleration(const string &id) -> dvec3;
    auto StepOrbitPoint(const string &id, const OrbitPoint &point, const double deltaTime) -> OrbitPoint;

public:
    SimulationState();

    auto StepToNextState(const double deltaTime) -> void;
};