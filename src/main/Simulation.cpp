#include "input/Keys.h"
#include "util/Constants.h"
#include "util/Log.h"
#include <GLFW/glfw3.h>
#include <main/Simulation.h>
#include <unordered_map>



namespace Simulation {

    namespace {
        const bfloat SPEED_MULTIPLIER = 10;
        const bfloat MIN_SPEED = 1;
        const bfloat MAX_SPEED = 10000000;

        bfloat time_step_size = 1;
        bfloat time_steps_per_frame = 10; 

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
    }

    auto Init() -> void {
        Keys::BindFunctionToKeyPress(GLFW_KEY_COMMA, DecreaseSimulationSpeed);
        Keys::BindFunctionToKeyPress(GLFW_KEY_PERIOD, IncreaseSimulationSpeed);
    }

    auto Integrate(const unordered_map<string, Massive> &massive_bodies, Body &body) -> void {
        // Loop through every body - we only need the massive bodies since massless bodies will have no effect on the body's acceleration
        bvec3 acceleration = bvec3(0, 0, 0);
        for (auto &pair : massive_bodies) {

            // Check that the massive body is not the target body
            // If this was the case, we would be trying to apply the body's gravitational force to itself...
            if (pair.second.GetId() == body.GetId()) {
                continue;
            }

            // Calculate force that the massive object is enacting on the body using Newton's Universal Law of Gravitation
            // and add the force to the total force vector
            bvec3 displacement = body.GetPosition() - pair.second.GetPosition();
            bvec3 direction = displacement.Normalize();
            bfloat distance = displacement.Magnitude();
            bfloat accelerationScalar = (GRAVITATIONAL_CONSTANT * pair.second.GetMass()) / boost::multiprecision::pow(distance, 2);
            acceleration -= direction * accelerationScalar;
        }

        // Apply acceleration to velocity
        body.AddVelocity(acceleration * time_step_size);
        body.AddPosition(body.GetVelocity() * time_step_size);
    }

    auto Integrate(unordered_map<string, Massive> &massive_bodies) -> void {
        for (int i = 0; i < time_steps_per_frame; i++) {
            for (auto &pair : massive_bodies) {
                Integrate(massive_bodies, pair.second);
            }
        }
    }

    auto SetTimeStepSize(bfloat size) -> void {
        time_step_size = size;
    }

    auto SetTimeStepsPerFrame(bfloat steps) -> void {
        time_steps_per_frame = steps;
    }
}