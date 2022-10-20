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
#include <rendering/interface/MassiveRender.h>
#include <main/Materials.h>
#include <unordered_map>



namespace Bodies {

    namespace {
        unordered_map<string, Massive> massiveBodies;
        unordered_map<string, Massless> masslessBodies;

        unordered_map<string, vector<dvec3>> positions;

        BodyType selectedType = NONE;
        string selected = "";

        float MASSLESS_MIN_ZOOM = 0.01;

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
                        selectedType = MASSIVE;
                        selected = pair.first;
                        MassiveRender::StartTransition(pair.second);
                }
            }
        }
    }

    auto Init() -> void {
        // Input
        Mouse::SetCallbackLeftDouble(SwitchSelectedBodyWhenSphereClicked);

        // Initial camera lock
        if (massiveBodies.size() != 0) {
            selectedType = MASSIVE;
            selected = massiveBodies.begin()->first;
        }
    }

    auto Update() -> void {
        // Integrate to get the future positions of all bodies
        positions = Simulation::Integrate(massiveBodies, masslessBodies);

        // Update transition target,so that the camera follows the target
        if (selectedType == MASSIVE) {
            MassiveRender::UpdateTransitionTarget(massiveBodies.at(selected));
        } else if (selectedType == MASSLESS) {
            MassiveRender::UpdateTransitionTarget(masslessBodies.at(selected));
        }
    }

    auto AddBody(const Massive &body) -> void {
        massiveBodies.insert(std::make_pair(body.GetId(), body));
        MassiveRender::AddBody(body);
    }

    auto AddBody(const Massless &body) -> void {
        // Massless bodies only need icons, so we don't need to worry about the Render namespace
        masslessBodies.insert(std::make_pair(body.GetId(), body));
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
            MassiveRender::StartTransition(massiveBodies.at(id));
        
        // If the id corresponds to a massless body
        } else if (masslessBodies.find(id) != masslessBodies.end()) {
            selectedType = MASSLESS;
            MassiveRender::StartTransition(masslessBodies.at(id));
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

    auto GetPositions() -> unordered_map<string, vector<dvec3>> {
        return positions;
    }
}