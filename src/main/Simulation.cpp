#include "input/Keys.h"
#include "main/OrbitPoint.h"
#include "util/Constants.h"
#include "util/Log.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/ext/quaternion_geometric.hpp>
#include <main/Simulation.h>
#include <unordered_map>
#include <utility>



namespace Simulation {

    namespace {
        const double SPEED_MULTIPLIER = 2;
        const double MIN_SPEED = 1.0/10000;
        const double MAX_SPEED = 10000000;

        const double INITIAL_TIME_STEP_SIZE = 100.0;
        const double INITIAL_TIME_STEPS_PER_FRAME = 100;

        double time_step_size = INITIAL_TIME_STEP_SIZE;
        double time_steps_per_frame = INITIAL_TIME_STEPS_PER_FRAME;

        auto IncreaseSimulationSpeed() -> void {
            // Check that this action won't increase the simulation speed above the maximum speed
            if ((time_step_size * SPEED_MULTIPLIER) > MAX_SPEED) {
                return;
            }
            time_step_size *= SPEED_MULTIPLIER;
        }

        auto DecreaseSimulationSpeed() -> void {
            // Check that this action won't decrease the simulation speed below the minimum speed
            if ((time_step_size / SPEED_MULTIPLIER) < MIN_SPEED) {
                return;
            }
            time_step_size /= SPEED_MULTIPLIER;
        }

        auto CalculateAcceleration(const unordered_map<string, Massive> &massive_bodies, const string &id, const dvec3 &bodyPosition) -> dvec3 {
            // Loop through every body - we only need the massive bodies since massless bodies will have no effect on the body's acceleration
            dvec3 acceleration = dvec3(0, 0, 0);
            for (const auto &pair : massive_bodies) {

                // Check that the massive body is not the target body
                // If this was the case, we would be trying to apply the body's gravitational force to itself...
                if (pair.second.GetId() == id) {
                    continue;
                }

                // Calculate force that the massive object is enacting on the body using Newton's Universal Law of Gravitation
                // and add the force to the total force vector
                dvec3 displacement = bodyPosition - pair.second.GetPosition();
                dvec3 direction = glm::normalize(displacement);
                double distance = glm::length(displacement);
                double accelerationScalar = (GRAVITATIONAL_CONSTANT * pair.second.GetMass()) / glm::pow(distance, 2);
                acceleration -= direction * accelerationScalar;
            }

            return acceleration;
        }
    }

    auto Init() -> void {
        Keys::BindFunctionToKeyPress(GLFW_KEY_COMMA, DecreaseSimulationSpeed);
        Keys::BindFunctionToKeyPress(GLFW_KEY_PERIOD, IncreaseSimulationSpeed);
    }

    auto Integrate(const unordered_map<string, Massive> &massiveBodies, const string &id, const double time_step, const OrbitPoint &point) -> OrbitPoint {
        dvec3 acceleration = CalculateAcceleration(massiveBodies, id, point.position);
        OrbitPoint newPoint = point;
        newPoint.velocity += acceleration * time_step;
        newPoint.position += point.velocity * time_step;
        return point;
    }

    auto Integrate(const unordered_map<string, Massive> &massiveBodies, Body &body) -> void {
        dvec3 acceleration = CalculateAcceleration(massiveBodies, body.GetId(), body.GetPosition());
        body.AddVelocity(acceleration * time_step_size);
        body.AddPosition(body.GetVelocity() * time_step_size);
    }

    auto Integrate(unordered_map<string, Massive> massiveBodies, unordered_map<string, Massless> masslessBodies) -> unordered_map<string, vector<OrbitPoint>> {
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
        for (int i = 0; i < time_steps_per_frame; i++) {

            // Massive
            for (auto &pair : massiveBodies) {
                points.at(pair.first).push_back(OrbitPoint{
                        .position=pair.second.GetPosition(), 
                        .velocity=pair.second.GetVelocity(),
                        .timeToNextPoint=time_step_size});
                Integrate(massiveBodies, pair.second);
            }

            // Massless
            for (auto &pair : masslessBodies) {
                points.at(pair.first).push_back(OrbitPoint{
                    .position=pair.second.GetPosition(), 
                    .velocity=pair.second.GetVelocity(),
                    .timeToNextPoint=time_step_size});
                Integrate(massiveBodies, pair.second);
            }
        }

        return points;
    }

    auto SetTimeStepSize(const double size) -> void {
        time_step_size = size;
    }

    auto SetTimeStepsPerFrame(const double size) -> void {
        time_steps_per_frame = size;
    }
}