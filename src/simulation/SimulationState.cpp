#include "SimulationState.h"
#include <glm/gtx/string_cast.hpp>
#include <simulation/OrbitPoint.h>
#include "rendering/geometry/Rays.h"

#include <main/Bodies.h>



SimulationState::SimulationState() {}

SimulationState::SimulationState(unordered_map<string, OrbitPoint> _points)
    : points(std::move(_points)) {}

auto SimulationState::CalculateIndividualAcceleration(const string &accelerationOf, const string &withRespectTo) -> dvec3 {
    // Calculate force that the massive object is enacting on the body using Newton's Universal Law of Gravitation
    // and add the force to the total force vector
    dvec3 accelerationOf_position = points.at(accelerationOf).position;
    dvec3 withRespectTo_position = points.at(withRespectTo).position;

    dvec3 displacement = accelerationOf_position - withRespectTo_position;
    dvec3 direction = glm::normalize(displacement);

    double withRespectTo_mass = Bodies::GetBody(withRespectTo).GetMass();
    double distance = glm::length(displacement);
    double accelerationScalar = GRAVITATIONAL_CONSTANT * withRespectTo_mass / glm::pow(distance, 2);

    return direction * accelerationScalar;
}

auto SimulationState::CalculateTotalAcceleration(const string &id) -> dvec3 {
    dvec3 acceleration = dvec3(0, 0, 0);
    for (const auto &pair : Bodies::GetMassiveBodies()) {
        if (pair.first == id) {
            continue;
        }

        acceleration -= CalculateIndividualAcceleration(id, pair.first);
    }

    return acceleration;
}

auto SimulationState::StepOrbitPoint(const string &id, OrbitPoint &point, const double timeStep) -> void {
    dvec3 acceleration = CalculateTotalAcceleration(id);
    point.velocity += acceleration * timeStep;
    point.position += point.velocity * timeStep;
}

auto SimulationState::StepToNextState(const double timeStep) -> void {
    for (auto &pair : points) {
        StepOrbitPoint(pair.first, pair.second, timeStep);
    }
}

auto SimulationState::Scale() -> void {
    for (auto &pair : points) {
        pair.second.position = Rays::Scale(pair.second.position);
    }
}
auto SimulationState::GetOrbitPoints() const -> unordered_map<string, OrbitPoint> {
    return points;
}
