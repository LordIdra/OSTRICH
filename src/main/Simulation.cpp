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
            //Log(INFO, displacement.x.str() + " " + displacement.y.str() + " " + displacement.z.str());
            //Log(INFO, direction.x.str() + " " + direction.y.str() + " " + direction.z.str());

            //Log(INFO, " ");
        }

        // Apply acceleration to velocity
        //Log(INFO, " ");
        body.AddVelocity(acceleration * TIME_STEP_SIZE);
        body.AddPosition(body.GetVelocity() * TIME_STEP_SIZE);
    }
}