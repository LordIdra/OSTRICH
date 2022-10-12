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
            vec3 direction = Rays::ScreenToWorld(Mouse::GetNormalizedPosition());

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
            dvec3(0, 0, 0),
            dvec3(0, 0, 0),
            Materials::earth,
            double(5.9722e24),     // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            double(6371.0e4)));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        AddBody(Massive(
            "the-moon",
            "The Moon",
            dvec3(double(1.4055e9), double(0), double(0)), // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            dvec3(double(0), double(0), double(0.570e3)),  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            Materials::moon1,
            double(0.07346e24),    // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            double(1737.4e3)));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)

        // Render
        Render::Init();
    }

    auto Update() -> void {
        Simulation::Integrate(massive_bodies);
        
        // Check that a body has been selected yet
        if (massive_bodies.find(selected) != massive_bodies.end()) {

            // Update transition target,so that the camera follows the target
            Render::UpdateTransitionTarget(massive_bodies.at(selected));
        }
    }

    auto GetSelectedBody() -> string {
        return selected;
    }

    auto SetSelectedBody(const string &id) -> void {
        selected = id;
        Render::StartTransition(massive_bodies.at(id));
    }

    auto GetMassiveBodies() -> unordered_map<string, Massive> {
        return massive_bodies;
    }

    auto GetMassiveBody(const string &id) -> Massive {
        return massive_bodies.at(id);
    }
}