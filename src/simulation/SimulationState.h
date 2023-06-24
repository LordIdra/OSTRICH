#pragma once

#include <simulation/OrbitPoint.h>
#include <util/Types.h>



class SimulationState {
private:
    unordered_map<string, OrbitPoint> points;
    unordered_map<string, dvec3> oldAcceleration;

    double accelerationLastTimeStep;

    auto CalculateIndividualAcceleration(const string &accelerationOf, const string &withRespectTo) -> dvec3;
    auto StepOrbitPoint(const string &id, OrbitPoint &point, const double timeStep) -> void;

public:
    SimulationState();
    SimulationState(unordered_map<string, OrbitPoint> _points);

    auto CalculateTotalAcceleration(const string &id) -> dvec3;
    auto StepToNextState(const double timeStep) -> void;
    auto Scale() -> void;
    auto GetOrbitPoints() const -> unordered_map<string, OrbitPoint>;
};