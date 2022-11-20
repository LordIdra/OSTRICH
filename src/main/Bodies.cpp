#include "Bodies.h"

#include "simulation/Simulation.h"
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
#include <rendering/world/OrbitPaths.h>
#include <simulation/OrbitPoint.h>

#include <GLFW/glfw3.h>
#include <string>



namespace Bodies {

    namespace {
        unordered_map<string, Body> bodies;
        unordered_map<string, Massive> massiveBodies;
        unordered_map<string, Massless> masslessBodies;
        vector<string> bodyIds;

        string selected;

        auto GetBodyAsReference(const string &id) -> Body& {
            return bodies.at(id);
        }

        auto GetMassiveAsReference(const string &id) -> Massive& {
            return massiveBodies.at(id);
        }

        auto GetMasslessAsReference(const string &id) -> Massless& {
            return masslessBodies.at(id);
        }

        auto IsBodyMassive(const string &id) -> bool {
            return (massiveBodies.find(id) != massiveBodies.end());
        }
    }

    auto PreReset() -> void {
        // Delete bodies
        massiveBodies.clear();
        bodies.clear();

        // Unselect bodies
        selected = "";
    }

    auto PostReset() -> void {
        if (!bodies.empty()) {
            selected = bodies.begin()->first;
        }
    }

    auto AddBody(const Massive &body) -> void {
        bodyIds.push_back(body.GetId());
        bodies.insert(std::make_pair(body.GetId(), body));
        massiveBodies.insert(std::make_pair(body.GetId(), body));
        MassiveRender::AddBody(body);
        Simulation::NewBodyReset();
    }

    auto AddBody(const Massless &body) -> void {
        bodyIds.push_back(body.GetId());
        bodies.insert(std::make_pair(body.GetId(), body));
        masslessBodies.insert(std::make_pair(body.GetId(), body));
        Simulation::NewBodyReset();
        OrbitPaths::NewBodyReset();
    }

    auto UpdateBody(const string &id, const OrbitPoint &point) -> void {
        GetBodyAsReference(id).SetPosition(point.position);
        GetBodyAsReference(id).SetVelocity(point.velocity);
        if (IsBodyMassive(id)) {
            GetMassiveAsReference(id).SetPosition(point.position);
            GetMassiveAsReference(id).SetVelocity(point.velocity);
        } else {
            GetMasslessAsReference(id).SetPosition(point.position);
            GetMasslessAsReference(id).SetVelocity(point.velocity);
        }
    }

    auto GetSelectedBodyId() -> string {
        return selected;
    }

    auto GetSelectedBody() -> const Body& {
        return bodies.at(selected);
    }

    auto GetMinZoom() -> float {
        if (IsBodyMassive(selected)) {
            return massiveBodies.at(selected).GetMinZoom();
        }
        return masslessBodies.at(selected).GetMinZoom();
    }

    auto SetSelectedBody(const string &id) -> void {
        // This function does not start a transition to the specified body, use CameraTransition::SetTargetBody for that
        selected = id;
    }

    auto IsBodySelected() -> bool {
        return selected != "";
    }

    auto GetMassiveBodies() -> const unordered_map<string, Massive>& {
        return massiveBodies;
    }

    auto GetMasslessBodies() -> const unordered_map<string, Massless>& {
        return masslessBodies;
    }

    auto GetBodies() -> const unordered_map<string, Body>& {
        return bodies;
    }

    auto GetBodyIds() -> const vector<string>& {
        return bodyIds;
    }

    auto GetMassiveBody(const string &id) -> const Massive& {
        return massiveBodies.at(id);
    }

    auto GetBody(const string &id) -> const Body& {
        return bodies.at(id);
    }

    auto GetBodyCount() -> unsigned int {
        return bodies.size();
    }
}