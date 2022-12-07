#include "SimulationState.h"
#include <glm/gtx/string_cast.hpp>
#include <simulation/OrbitPoint.h>
#include "rendering/geometry/Rays.h"

#include <main/Bodies.h>



SimulationState::SimulationState() {}

SimulationState::SimulationState(unordered_map<string, OrbitPoint> _points)
    : points(std::move(_points)) {
        for (const auto &pair : points) {
            oldAcceleration.insert(std::make_pair(pair.first, 0));
        }
    }

auto SimulationState::CalculateIndividualAcceleration(const string &accelerationOf, const string &withRespectTo) -> dvec3 {
    // Calculate force that the massive object is enacting on the body using Newton's Universal Law of Gravitation
    // and add the force to the total force vector
    ZoneNamedN(CalculateIndividualAcceleration1, "CalculateIndividualAcceleration1", true);
    const dvec3 &accelerationOf_position = points.at(accelerationOf).position;
    const dvec3 &withRespectTo_position = points.at(withRespectTo).position;

    ZoneNamedN(CalculateIndividualAcceleration2, "CalculateIndividualAcceleration2", true);
    const dvec3 displacement = accelerationOf_position - withRespectTo_position;
    const dvec3 direction = glm::normalize(displacement);

    ZoneNamedN(CalculateIndividualAcceleration3, "CalculateIndividualAcceleration3", true);
    const double withRespectTo_mass = Bodies::GetBody(withRespectTo).GetMass();
    const double distance = glm::length(displacement);
    const double accelerationScalar = GRAVITATIONAL_CONSTANT * withRespectTo_mass / (distance * distance);
    
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
    // https://web.archive.org/web/20120713004111/http://wiki.vdrift.net:80/Numerical_Integration
    ZoneNamedN(STEP_ORBIT_POINT_1, "StepOrbitPoint1", true);
    point.position += (point.velocity * timeStep) + (0.5 * oldAcceleration.at(id) * timeStep * timeStep);
    point.velocity += 0.5 * oldAcceleration.at(id) * timeStep;
    ZoneNamedN(STEP_ORBIT_POINT_2, "StepOrbitPoint2", true);
    dvec3 newAcceleration = CalculateTotalAcceleration(id);
    ZoneNamedN(STEP_ORBIT_POINT_3, "StepOrbitPoint3", true);
    point.velocity += 0.5 * newAcceleration * timeStep;
    oldAcceleration[id] = newAcceleration;

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
