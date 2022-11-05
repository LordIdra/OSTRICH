#include "Bodies.h"
#include "main/Simulation.h"
#include "util/Log.h"

#include <bodies/Body.h>
#include <bodies/Massive.h>
#include <bodies/Massless.h>
#include <rendering/camera/Camera.h>
#include <rendering/geometry/Rays.h>
#include <rendering/shaders/Program.h>
#include <rendering/geometry/Transition.h>
#include <input/Keys.h>
#include <input/Mouse.h>
#include <rendering/interface/Bodies/MassiveRender.h>
#include <main/OrbitPoint.h>




namespace Bodies {

    namespace {
        unordered_map<string, Massive> massiveBodies;
        unordered_map<string, Massless> masslessBodies;

        unordered_map<string, vector<OrbitPoint>> points;

        BodyType selectedType = BODY_TYPE_NONE;
        string selected;

        const float INITIAL_MASSLESS_MIN_ZOOM = 0.01;
        float masslessMinZoom = INITIAL_MASSLESS_MIN_ZOOM;

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
                        selectedType = BODY_TYPE_MASSIVE;
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
        if (!massiveBodies.empty()) {
            selectedType = BODY_TYPE_MASSIVE;
            selected = massiveBodies.begin()->first;
        }
    }

    auto Update() -> void {
        // Integrate to get the future positions of all bodies
        points = Simulation::Integrate();

        // Update transition target,so that the camera follows the target
        if (selectedType == BODY_TYPE_MASSIVE) {
            MassiveRender::UpdateTransitionTarget(massiveBodies.at(selected));
        } else if (selectedType == BODY_TYPE_MASSLESS) {
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

    auto GetSelectedType() -> BodyType {
        return selectedType;
    }

    auto GetMinZoom() -> float {
        if (selectedType == BODY_TYPE_MASSIVE) {
            return Bodies::GetMassiveBody(Bodies::GetSelectedBody()).GetScaledRadius() * ZOOM_RADIUS_MULTIPLIER;
        }
        return masslessMinZoom;
    }

    auto SetSelectedBody(const string &id) -> void {
        selected = id;

        // If the id corresponds to a massive body
        if (massiveBodies.find(id) != massiveBodies.end()) {
            selectedType = BODY_TYPE_MASSIVE;
            MassiveRender::StartTransition(massiveBodies.at(id));
        
        // If the id corresponds to a massless body
        } else if (masslessBodies.find(id) != masslessBodies.end()) {
            selectedType = BODY_TYPE_MASSLESS;
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

    auto GetBody(const string &id) -> Body {
        if (massiveBodies.find(id) != massiveBodies.end()) {
            return massiveBodies.at(id);
        }
        // Yes, technically the id may not exist, but this is REALLY unlikely unless some code is very obviously wrong in other ways
        return masslessBodies.at(id);
    }

    auto GetPositions() -> unordered_map<string, vector<OrbitPoint>> {
        return points;
    }
}