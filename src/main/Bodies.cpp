#include "Bodies.h"
#include "main/Simulation.h"
#include "util/Log.h"

#include <bodies/BodyType.h>
#include <bodies/Body.h>
#include <bodies/Massive.h>
#include <bodies/Massless.h>
#include <glm/gtx/string_cast.hpp>
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
        unordered_map<string, Massive> massiveBodies;
        unordered_map<string, Massless> masslessBodies;

        BodyType selectedType;
        string selected;

        float MASSLESS_MIN_ZOOM = 0.01;

        auto AddBody(const Massive &body) -> void {
            massiveBodies.insert(std::make_pair(body.GetId(), body));
            Render::AddBody(body);
        }

        auto AddBody(const Massless &body) -> void {
            // Massless bodies only need icons, so we don't need to worry about the Render namespace
            masslessBodies.insert(std::make_pair(body.GetId(), body));
        }

        auto SwitchSelectedBodyWhenSphereClicked() -> void {
            // This function only cares about the spheres representing massive bodies
            // Seletcing via icons is handles in the Icons namespace
            // Find the camera direction
            vec3 direction = Rays::ScreenToWorld(Mouse::GetScreenPosition());

            // Loop through every body
            for (auto &pair : massiveBodies) {

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
        Mouse::SetCallbackLeftDouble(SwitchSelectedBodyWhenSphereClicked);

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
        AddBody(Massless(
            "spacecraft",
            "Spacecraft",
            dvec3(double(-1.4055e9), double(0), double(0)),  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            dvec3(double(0), double(0), double(0.570e3))));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        
        selectedType = MASSIVE;
        selected = massiveBodies.begin()->first;
        
        // Render
        Render::Init();
    }

    auto Update() -> void {
        Simulation::Integrate(massiveBodies);
        
        // Check that a body has been selected yet
        if (massiveBodies.find(selected) != massiveBodies.end()) {

            // Update transition target,so that the camera follows the target
            Render::UpdateTransitionTarget(massiveBodies.at(selected));
        }
    }

    auto GetSelectedBody() -> string {
        return selected;
    }

    auto GetMinZoom() -> float {
        if (selectedType == MASSIVE) {
            return Bodies::GetMassiveBody(Bodies::GetSelectedBody()).GetScaledRadius() * ZOOM_RADIUS_MULTIPLIER;
        } else {
            return MASSLESS_MIN_ZOOM;
        }
    }

    auto SetSelectedBody(const string &id) -> void {
        selected = id;

        // If the id corresponds to a massive body
        if (massiveBodies.find(id) != massiveBodies.end()) {
            selectedType = MASSIVE;
            Render::StartTransition(massiveBodies.at(id));
        
        // If the id corresponds to a massless body
        } else if (masslessBodies.find(id) != masslessBodies.end()) {
            selectedType = MASSLESS;
            Render::StartTransition(masslessBodies.at(id));
        }
    }

    auto GetMassiveBodies() -> unordered_map<string, Massive> {
        return massiveBodies;
    }

    auto GetMasslessBodies() -> unordered_map<string, Massless> {
        return masslessBodies;
    }

    auto GetMassiveBody(const string &id) -> Massive {
        return massiveBodies.at(id);
    }

    auto GetMasslessBody(const string &id) -> Massless {
        return masslessBodies.at(id);
    }
}