#include "Simulation.h"

#include "input/Keys.h"
#include "main/Bodies.h"
#include "main/OrbitPoint.h"
#include "util/Constants.h"
#include "util/Log.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <unordered_map>
#include <utility>



namespace Simulation {

    namespace {
        const double INITIAL_TIME_STEP_SIZE = 5000;
        const double INITIAL_TIME_STEP_COUNT = 10000;
        const double INITIAL_SIMULATION_SPEED = 1;

        double timeStepSize = INITIAL_TIME_STEP_SIZE;
        double timeStepsPerFrame = INITIAL_TIME_STEP_COUNT;
        double simulationSpeed = INITIAL_SIMULATION_SPEED;
        double simulationSpeedMultiplier = INITIAL_SIMULATION_SPEED;

        double timeStep = 0;

        auto IncreaseSimulationSpeed() -> void {
            // Check that this action won't increase the simulation speed above the maximum speed
            if (simulationSpeedMultiplier >= MAX_MULTIPLIER) {
                return;
            }
            simulationSpeedMultiplier += 1;
            simulationSpeed *= SPEED_MULTIPLIER;
        }

        auto DecreaseSimulationSpeed() -> void {
            // Check that this action won't decrease the simulation speed below the minimum speed
            if (simulationSpeedMultiplier <= MIN_MULTIPLIER) {
                return;
            }
            simulationSpeedMultiplier -= 1;
            simulationSpeed /= SPEED_MULTIPLIER;
        }
    }

    auto Init() -> void {
        Keys::BindFunctionToKeyPress(GLFW_KEY_COMMA, DecreaseSimulationSpeed);
        Keys::BindFunctionToKeyPress(GLFW_KEY_PERIOD, IncreaseSimulationSpeed);
    }

    auto UpdateTime(const float deltaTime) -> void {
        timeStep += deltaTime * simulationSpeed;
    }

    auto CalculateAcceleration(const unordered_map<string, Massive> &massiveBodies, const string &id, const dvec3 &position) -> dvec3 {
        // Loop through every body - we only need the massive bodies since massless bodies will have no effect on the body's acceleration
        dvec3 acceleration = dvec3(0, 0, 0);
        for (const auto &pair : massiveBodies) {

            // Check that the massive body is not the target body
            // If this was the case, we would be trying to apply the body's gravitational force to itself...
            if (pair.second.GetId() == id) {
                continue;
            }

            // Calculate force that the massive object is enacting on the body using Newton's Universal Law of Gravitation
            // and add the force to the total force vector
            dvec3 displacement = position - pair.second.GetPosition();
            dvec3 direction = glm::normalize(displacement);
            double distance = glm::length(displacement);
            double accelerationScalar = (GRAVITATIONAL_CONSTANT * pair.second.GetMass()) / glm::pow(distance, 2);
            acceleration -= direction * accelerationScalar;
        }

        return acceleration;
    }

    auto GenerateNextOrbitPoint(const unordered_map<string, Massive> &massiveBodies, const string &id, const double time_step, const OrbitPoint &point) -> OrbitPoint {
        dvec3 acceleration = CalculateAcceleration(massiveBodies, id, point.position);
        OrbitPoint newPoint = point;
        newPoint.velocity += acceleration * time_step;
        newPoint.position += point.velocity * time_step;
        return newPoint;
    }

    auto UpdateBodyPosition(const unordered_map<string, Massive> &massiveBodies, Body &body) -> void {
        dvec3 acceleration = CalculateAcceleration(massiveBodies, body.GetId(), body.GetPosition());
        body.AddVelocity(acceleration * timeStepSize);
        body.AddPosition(body.GetVelocity() * timeStepSize);
    }

    auto RegenerateFuturePoints() -> unordered_map<string, vector<OrbitPoint>> {
        unordered_map<string, Massive> massiveBodies = Bodies::GetMassiveBodies();
        unordered_map<string, Massless> masslessBodies = Bodies::GetMasslessBodies();
        unordered_map<string, vector<OrbitPoint>> points;

        // Fill the map with keys that correspond to all massive and massless bodies
        // Massive
        for (const auto &pair : massiveBodies) {
            points.insert(std::make_pair(pair.first, vector<OrbitPoint>()));
        }

        // Massless
        for (const auto &pair : masslessBodies) {
            points.insert(std::make_pair(pair.first, vector<OrbitPoint>()));
        }

        // Integrate for all bodies and add positions
        for (int i = 0; i < timeStepsPerFrame; i++) {

            // Massive
            for (auto &pair : massiveBodies) {
                points.at(pair.first).push_back(OrbitPoint{
                    .position=pair.second.GetPosition(), 
                    .velocity=pair.second.GetVelocity()});
                UpdateBodyPosition(massiveBodies, pair.second);
            }

            // Massless
            for (auto &pair : masslessBodies) {
                points.at(pair.first).push_back(OrbitPoint{
                    .position=pair.second.GetPosition(), 
                    .velocity=pair.second.GetVelocity()});
                UpdateBodyPosition(massiveBodies, pair.second);
            }
        }

        return points;
    }

    auto GetKineticEnergy(const Body &body) -> double {
        return 0.5 * body.GetMass() * pow(glm::length(body.GetVelocity()), 2);
    }

    auto GetPotentialEnergy(const Body &body) -> double {
        double energy = 0;
        for (const auto &pair : Bodies::GetMassiveBodies()) {
            if (pair.first == body.GetId()) {
                continue;
            }
            double massProduct = body.GetMass() * pair.second.GetMass();
            double distance = glm::distance(body.GetPosition(), pair.second.GetPosition());
            energy -= (GRAVITATIONAL_CONSTANT * massProduct) / distance;
        }
        return energy;
    }

    auto GetTotalEnergy(const Body &body) -> double {
        return GetKineticEnergy(body) + GetPotentialEnergy(body);
    }

    auto SetTimeStepSize(const double size) -> void {
        timeStepSize = size;
    }

    auto SetTimeStepsPerFrame(const double size) -> void {
        timeStepsPerFrame = size;
    }

    auto GetTimeStepSize() -> double {
        return timeStepSize;
    }

    auto GetTimeStep() -> double {
        return timeStep;
    }

    auto GetSimulationSpeed() -> double {
        return simulationSpeed;
    }

    auto GetSimulationSpeedMultiplier() -> double {
        return simulationSpeedMultiplier;
    }

    auto GetSimulationKineticEnergy() -> double {
        double energy = 0;
        for (const auto &pair : Bodies::GetMassiveBodies())  { energy += GetKineticEnergy(pair.second); }
        for (const auto &pair : Bodies::GetMasslessBodies()) { energy += GetKineticEnergy(pair.second); }
        return energy;
    }

    auto GetSimulationPotentialEnergy() -> double {
        double energy = 0;
        for (const auto &pair : Bodies::GetMassiveBodies())  { energy += GetPotentialEnergy(pair.second); }
        for (const auto &pair : Bodies::GetMasslessBodies()) { energy += GetPotentialEnergy(pair.second); }
        return energy;
    }

    auto GetSimulationTotalEnergy() -> double {
        return GetSimulationKineticEnergy() + GetSimulationPotentialEnergy();
    }
}