#include "Bodies.h"
#include "main/Simulation.h"
#include "util/Log.h"

#include <bodies/Body.h>
#include <bodies/Massive.h>
#include <rendering/camera/Camera.h>
#include <rendering/geometry/Rays.h>
#include <rendering/shaders/Program.h>
#include <rendering/geometry/Transition.h>
#include <input/Keys.h>
#include <input/Mouse.h>
#include <main/Render.h>
#include <main/Materials.h>



namespace Bodies {

    namespace {
        unordered_map<string, Massive> massive_bodies;

        string selected;

        auto AddBody(const Massive &body) -> void {
            massive_bodies.insert(std::make_pair(body.GetId(), body));
            Render::AddBody(body);
        }

        auto SwitchSelectedBody() -> void {
            // Find the camera direction
            vec3 direction = Rays::ScreenToWorld(Mouse::GetPosition());

            // Loop through every body
            for (auto &pair : massive_bodies) {

                // Check if the camera direction intersects the body
                if (Rays::IntersectsSphere(
                    Camera::GetPosition(), 
                    direction, 
                    pair.second.GetScaledPosition(), 
                    pair.second.GetScaledRadius())) {

                        // If so, update the selected body and start a transition
                        selected = pair.first;
                        Render::StartTransition(pair.second);
                }
            }
        }
    }

    auto Init() -> void {
        // Input
        Mouse::SetCallbackLeftDouble(SwitchSelectedBody);

        // Bodies
        AddBody(Massive(
            "earth",
            "Earth",
            bvec3(0, 0, 0),
            bvec3(0, 0, 0),
            Materials::earth,
            bfloat(5.9722e24),     // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            bfloat(6371.0e4)));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        AddBody(Massive(
            "the-moon",
            "The Moon",
            bvec3(bfloat(1.4055e9), bfloat(0), bfloat(0)), // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            bvec3(bfloat(0), bfloat(0), bfloat(0.570e3)),  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            Materials::moon1,
            bfloat(0.07346e24),    // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            bfloat(1737.4e4)));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        AddBody(Massive(
            "the-moon-2",
            "The Moon 2.0",
            bvec3(bfloat(2.4055e9), bfloat(0), bfloat(0)), // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            bvec3(bfloat(0), bfloat(0), bfloat(-0.470e3)),   // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            Materials::moon2,
            bfloat(0.07346e26),    // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            bfloat(9500e4)));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)

        // Render
        Render::Init();
    }

    auto Update() -> void {
        // Check that a body has been selected yet
        if (massive_bodies.find(selected) != massive_bodies.end()) {

            // Update transition target,so that the camera follows the target
            Render::UpdateTransitionTarget(massive_bodies.at(selected));
        }

        Simulation::Integrate(massive_bodies);
    }

    auto GetMassiveBody(const string &id) -> Massive {
        return massive_bodies.at(id);
    }
}