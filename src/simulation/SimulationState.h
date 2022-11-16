#pragma once

#include <simulation/OrbitPoint.h>
#include <util/Types.h>



class SimulationState {
private:
    unordered_map<string, OrbitPoint> points;

    auto CalculateIndividualAcceleration(const string &accelerationOf, const string &withRespectTo) -> dvec3;
    auto CalculateTotalAcceleration(const string &id) -> dvec3;
    auto StepOrbitPoint(const string &id, OrbitPoint &point, const double deltaTime) -> void;

public:
    SimulationState(unordered_map<string, OrbitPoint> _points);

    auto StepToNextState(const double deltaTime) -> void;
    auto Scale() -> void;
    auto GetOrbitPoints() const -> unordered_map<string, OrbitPoint>;
};