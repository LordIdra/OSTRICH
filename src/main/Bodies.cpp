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
#include <rendering/world/MassiveRender.h>
#include <main/OrbitPoint.h>

#include <GLFW/glfw3.h>
#include <string>




namespace Bodies {

    namespace {
        unordered_map<string, Massive> massiveBodies;
        unordered_map<string, Massless> masslessBodies;

        BodyType selectedType = BODY_TYPE_NONE;
        string selected;

        const float INITIAL_MASSLESS_MIN_ZOOM = 0.01;

        float masslessMinZoom = INITIAL_MASSLESS_MIN_ZOOM;

        auto GetBodyAsReference(const string &id) -> Body& {
            if (massiveBodies.find(id) != massiveBodies.end()) {
                return massiveBodies.at(id);
            }
            // Yes, technically the id may not exist, but this is REALLY unlikely unless some code is very obviously wrong in other ways
            return masslessBodies.at(id);
        }
    }

    auto PreReset() -> void {
        // Delete bodies
        massiveBodies.clear();
        masslessBodies.clear();

        // Unselect bodies
        selectedType = BODY_TYPE_NONE;
        selected = "";
    }

    auto PostReset() -> void {
        if (!massiveBodies.empty()) {
            selectedType = BODY_TYPE_MASSIVE;
            selected = massiveBodies.begin()->first;
        }
    }

    auto AddBody(const Massive &body) -> void {
        massiveBodies.insert(std::make_pair(body.GetId(), body));
        MassiveRender::AddBody(body);
    }

    auto AddBody(const Massless &body) -> void {
        masslessBodies.insert(std::make_pair(body.GetId(), body));
    }

    auto UpdateBody(const string &id, const OrbitPoint &point) -> void {
        GetBodyAsReference(id).SetPosition(point.position);
        GetBodyAsReference(id).SetVelocity(point.velocity);
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
        // important: this function does not start a transition to the specified body, use CameraTransition::SetTargetBody instead of this
        selected = id;

        // If the id corresponds to a massive body
        if (massiveBodies.find(id) != massiveBodies.end()) {
            selectedType = BODY_TYPE_MASSIVE;
            
        // If the id corresponds to a massless body
        } else if (masslessBodies.find(id) != masslessBodies.end()) {
            selectedType = BODY_TYPE_MASSLESS;
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
}