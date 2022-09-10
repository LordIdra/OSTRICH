#include "util/Constants.h"
#include <main/Simulation.h>



namespace Simulation {

    auto Integrate(const vector<Massive> &bodies, Body &body) -> void {
        // Loop through every body - we only need the massive bodies since massless bodies will have no effect on the body's acceleration
        bvec3 acceleration = bvec3(1, 1, 1);
        for (const Massive &massive : bodies) {
            
            // Calculate force that the massive object is enacting on the body using Newton's Universal Law of Gravitation
            // and add the force to the total force vector
            bvec3 displacement = body.GetPosition() - massive.GetPosition();
            acceleration += displacement.Power(-1) * (GRAVITATIONAL_CONSTANT * massive.GetMass());
        }

        // Apply acceleration to velocity
        body.AddVelocity(acceleration * TIME_STEP_SIZE);
        body.AddPosition(body.GetVelocity() * TIME_STEP_SIZE);
    }
}