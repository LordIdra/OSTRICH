#include "Bodies.h"

#include <bodies/Body.h>
#include <bodies/Massive.h>
#include <rendering/VAO.h>
#include <rendering/camera/Camera.h>
#include <rendering/geometry/Rays.h>
#include <rendering/shaders/Program.h>
#include <rendering/geometry/Transition.h>
#include <input/Keys.h>
#include <input/Mouse.h>
#include <main/Render.h>



namespace Bodies {

    namespace {
        unordered_map<string, Massive> massive_bodies;

        string selected;

        auto AddBody(const Massive &body) -> void {
            massive_bodies.insert(std::make_pair(body.GetName(), body));
            Render::AddBody(body);
        }

        auto SwitchSelectedBody() -> void {
            // Find the camera direction
            vec3 direction = Rays::ScreenToWorld(Mouse::GetPosition());

            // Loop through every body
            for (const auto &pair : massive_bodies) {

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
            bfloat(5.9722e24),     // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            bfloat(6371.0e3)));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        AddBody(Massive(
            "the-moon",
            "The Moon",
            bvec3(bfloat(0.4055e8), bfloat(0), bfloat(0)), // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            bvec3(bfloat(0), bfloat(0), bfloat(0.970e3)),  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            bfloat(0.07346e24),    // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            bfloat(1737.4e3)));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        AddBody(Massive(
            "the-moon-2",
            "The Moon 2.0",
            bvec3(bfloat(-0.4055e8), bfloat(0), bfloat(0)), // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            bvec3(bfloat(0), bfloat(0), bfloat(0.970e3)),   // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            bfloat(0.07346e24),    // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            bfloat(2500e3)));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)

        // Render
        Render::Init();
    }
}