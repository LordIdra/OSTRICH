#include "util/Constants.h"
#include "util/Log.h"
#include <main/Simulation.h>
#include <unordered_map>



namespace Simulation {

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
        body.AddVelocity(acceleration * TIME_STEP_SIZE);
        body.AddPosition(body.GetVelocity() * TIME_STEP_SIZE);
    }

    auto Integrate(unordered_map<string, Massive> &massive_bodies) -> void {
        for (int i = 0; i < TIME_STEPS_PER_FRAME; i++) {
            for (auto &pair : massive_bodies) {
                Integrate(massive_bodies, pair.second);
            }
        }
    }
}