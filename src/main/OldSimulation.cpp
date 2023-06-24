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

#include <depend/tracy-0.9/public/tracy/Tracy.hpp>
#include <vector>



namespace Simulation {

    namespace {
        const double INITIAL_TIME_STEP_SIZE = 500;
        const double INITIAL_TIME_STEPS_PER_FRAME = 500;
        const double INITIAL_SIMULATION_SPEED_MULTIPLIER = 1;

        const unsigned int MAX_PAST_POINTS = 1000;
        const unsigned int MAX_FUTURE_POINTS = 1000;

        double timeStepSize = INITIAL_TIME_STEP_SIZE;
        double timeStepsPerFrame = INITIAL_TIME_STEPS_PER_FRAME;
        double simulationSpeed = INITIAL_SIMULATION_SPEED_MULTIPLIER;
        double simulationSpeedMultiplier = INITIAL_SIMULATION_SPEED_MULTIPLIER;

        double timeSinceLastStateUpdate = 0;

        int futurePointCount = 0;

        double timeStep = 0;

        unordered_map<string, vector<OrbitPoint>> futurePoints;
        unordered_map<string, vector<OrbitPoint>> pastPoints;

        vector<string> bodiesWithSignificantMass;

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

        auto UpdateBody(const string &id) -> void {
            vector<OrbitPoint> &futurePointVector = futurePoints.at(id);
            vector<OrbitPoint> &pastPointVector = pastPoints.at(id);
            const OrbitPoint &newPoint = futurePointVector.at(1);

            // Move body to next point
            Bodies::UpdateBody(id, newPoint);

            // The point we just moved to is now also a past point 
            pastPointVector.push_back(newPoint);
            futurePointVector.erase(futurePointVector.begin());

            // Check that we don't have too many past points now
            if (pastPointVector.size() > MAX_PAST_POINTS) {
                pastPointVector.erase(pastPointVector.begin());
            }
        }

        auto IncrementBodyOrbitPoints() -> void {
            // Move every body to the next orbit point, spanning the time period
            // that the last frame encompassed
            // For example, if TimeStepSize is 10 and in one frame we moved 100 time steps,
            // we would do 11 updates
            while (timeSinceLastStateUpdate >= Simulation::GetTimeStepSize()) {
                timeSinceLastStateUpdate -= Simulation::GetTimeStepSize();
                futurePointCount -= 1;
                for (auto &pair : Bodies::GetMassiveBodies())  { UpdateBody(pair.first); }
                for (auto &pair : Bodies::GetMasslessBodies()) { UpdateBody(pair.first); }
            }
        }
    }

    auto Init() -> void {
        Keys::BindFunctionToKeyPress(GLFW_KEY_COMMA, DecreaseSimulationSpeed);
        Keys::BindFunctionToKeyPress(GLFW_KEY_PERIOD, IncreaseSimulationSpeed);
    }

    auto Update(const double deltaTime) -> void {
        // Update time
        timeStep += deltaTime * simulationSpeed;
        timeSinceLastStateUpdate += deltaTime * simulationSpeed;

        IncrementBodyOrbitPoints();

        // The rest of the function will be updating orbit points
        // so check first if we actually need to add any more orbit points
        if (futurePointCount >= MAX_FUTURE_POINTS) {
            return;
        }

        // Integrate for all bodies and add positions
        while (futurePointCount < MAX_FUTURE_POINTS) {

            if (futurePointCount >= MAX_FUTURE_POINTS) {
                return;
            }

            // Massive
            for (auto &pair : Bodies::GetMassiveBodies()) {
                OrbitPoint oldPoint = futurePoints.at(pair.first).at(futurePoints.at(pair.first).size()-1);
                OrbitPoint newPoint = GenerateNextOrbitPoint(pair.first, oldPoint);
                futurePoints.at(pair.first).push_back(newPoint);
            }

            // Massless
            for (auto &pair : Bodies::GetMasslessBodies()) {
                OrbitPoint oldPoint = futurePoints.at(pair.first).at(futurePoints.at(pair.first).size()-1);
                OrbitPoint newPoint = GenerateNextOrbitPoint(pair.first, oldPoint);
                futurePoints.at(pair.first).push_back(newPoint);
            }

            futurePointCount++;
        }
    }

    auto PreReset() -> void {
        timeStepSize = INITIAL_TIME_STEP_SIZE;
        timeStepsPerFrame = INITIAL_TIME_STEPS_PER_FRAME;
        simulationSpeed = INITIAL_SIMULATION_SPEED_MULTIPLIER;
        simulationSpeedMultiplier = INITIAL_SIMULATION_SPEED_MULTIPLIER;
        timeStep = 0;
        timeSinceLastStateUpdate = 0;
        pastPoints.clear();
        futurePoints.clear();
    }

    auto PostReset() -> void {
        // Fill the point map with keys that correspond to all massive and massless bodies
        // Massive
        for (const auto &pair : Bodies::GetMassiveBodies()) {
            futurePoints.insert(std::make_pair(pair.first, vector<OrbitPoint>{OrbitPoint{pair.second.GetPosition(), pair.second.GetVelocity()}}));
            pastPoints.insert(std::make_pair(pair.first, vector<OrbitPoint>()));
            bodiesWithSignificantMass.push_back(pair.first);
        }

        // Massless
        for (const auto &pair : Bodies::GetMasslessBodies()) {
            futurePoints.insert(std::make_pair(pair.first, vector<OrbitPoint>{OrbitPoint{pair.second.GetPosition(), pair.second.GetVelocity()}}));
            pastPoints.insert(std::make_pair(pair.first, vector<OrbitPoint>()));
        }
    }

    auto CalculateLatestAcceleration(const string &id, const dvec3 &position) -> dvec3 {
        // Loop through every body - we only need the massive bodies since massless bodies will have no effect on the body's acceleration
        dvec3 acceleration = dvec3(0, 0, 0);
        for (const string &otherId : bodiesWithSignificantMass) {

            Body otherBody = Bodies::GetBody(otherId);
            dvec3 otherPosition = futurePoints.at(otherId).at(futurePoints.at(otherId).size()-1).position;

            // Check that the massive body is not the target body
            // If this was the case, we would be trying to apply the body's gravitational force to itself...
            if (otherId == id) {
                continue;
            }

            // Calculate force that the massive object is enacting on the body using Newton's Universal Law of Gravitation
            // and add the force to the total force vector
            dvec3 displacement = position - otherPosition;
            dvec3 direction = glm::normalize(displacement);
            double distance = glm::length(displacement);
            double accelerationScalar = (GRAVITATIONAL_CONSTANT * otherBody.GetMass()) / glm::pow(distance, 2);
            acceleration -= direction * accelerationScalar;
        }

        return acceleration;
    }

    auto GenerateNextOrbitPoint(const string &id, const OrbitPoint &point) -> OrbitPoint {
        dvec3 acceleration = CalculateLatestAcceleration(id, point.position);
        OrbitPoint newPoint = point;
        newPoint.velocity += acceleration * timeStep;
        newPoint.position += point.velocity * timeStep;
        return newPoint;
    }

    auto GetPastOrbitPoints() -> unordered_map<string, vector<OrbitPoint>> {
        return pastPoints;
    }

    auto GetFutureOrbitPoints() -> unordered_map<string, vector<OrbitPoint>> {
        return futurePoints;
    }

    auto GetKineticEnergy(const Body &body) -> double {
    return 0.5 * body.GetMass() * pow(glm::length(body.GetVelocity()), 2); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
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

    auto SetTimeStep(const double step) -> void {
        timeStep = step;
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