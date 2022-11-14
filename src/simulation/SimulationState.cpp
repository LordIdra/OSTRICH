#include "SimulationState.h"
#include "main/OrbitPoint.h"

#include <main/Bodies.h>



SimulationState::SimulationState() {}

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

        acceleration -= CalculateIndividualAcceleration(pair.first, id);
    }

    return acceleration;
}

auto SimulationState::StepOrbitPoint(const string &id, const OrbitPoint &point, const double deltaTime) -> OrbitPoint {
    dvec3 acceleration = CalculateTotalAcceleration(id);
    OrbitPoint newPoint = point;

    newPoint.velocity += acceleration * deltaTime;
    newPoint.position += point.velocity * deltaTime;
    
    return newPoint;
}

auto SimulationState::StepToNextState(const double deltaTime) -> void {
    for (auto &pair : points) {
        StepOrbitPoint(pair.first, pair.second, deltaTime);
    }
}