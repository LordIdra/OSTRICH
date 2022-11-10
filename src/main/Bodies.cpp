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

        unordered_map<string, vector<OrbitPoint>> pastPoints;
        unordered_map<string, vector<OrbitPoint>> futurePoints;

        const unsigned int MAX_PAST_POINTS = 600;

        BodyType selectedType = BODY_TYPE_NONE;
        string selected;

        const float INITIAL_MASSLESS_MIN_ZOOM = 0.01;

        float masslessMinZoom = INITIAL_MASSLESS_MIN_ZOOM;

        double timeSinceLastOrbitPointUpdate = 0;

        auto UpdateBody(Body &body) -> void {
            vector<OrbitPoint> &futurePointVector = futurePoints.at(body.GetId());
            vector<OrbitPoint> &pastPointVector = pastPoints.at(body.GetId());
            const OrbitPoint &newPoint = futurePointVector.at(1);

            // Move body to next point
            body.SetPosition(newPoint.position);
            body.SetVelocity(newPoint.velocity);

            // The point we just moved to is now also a past point 
            pastPointVector.push_back(newPoint);
            futurePointVector.erase(futurePointVector.begin());

            // Check that we don't have too many past points now
            if (pastPointVector.size() > MAX_PAST_POINTS) {
                pastPointVector.erase(pastPointVector.begin());
            }
        }

        auto IncrementBodyOrbitPoint() -> void {
            // Continually move every body to the next orbit point, spanning the time period
            // that the last frame encompassed
            // For example, if TimeStepSize is 10 and in one frame we moved 100 time steps,
            // we would do 11 updates
            while (timeSinceLastOrbitPointUpdate >= Simulation::GetTimeStepSize()) {
                timeSinceLastOrbitPointUpdate -= Simulation::GetTimeStepSize();
                for (auto &pair : massiveBodies)  { UpdateBody(pair.second); }
                for (auto &pair : masslessBodies) { UpdateBody(pair.second); }
            }
        }
    }

    auto Reset() -> void {
        // Delete bodies
        massiveBodies.clear();
        masslessBodies.clear();

        // Delete points
        pastPoints.clear();
        futurePoints.clear();

        // Unselect bodies
        selectedType = BODY_TYPE_NONE;
        selected = "";

        // Reset time since last update
        timeSinceLastOrbitPointUpdate = 0;
    }

    auto InitializeSelectedBody() -> void {
        if (!massiveBodies.empty()) {
            selectedType = BODY_TYPE_MASSIVE;
            selected = massiveBodies.begin()->first;
        }
    }

    auto Update(const double deltaTime) -> void {
        // Integrate to get the future positions of all bodies
        futurePoints = Simulation::RegenerateFuturePoints();
        timeSinceLastOrbitPointUpdate += deltaTime * Simulation::GetSimulationSpeed();

        // Move bodies to next orbit point(s) if it's time to do so
        IncrementBodyOrbitPoint();
    }

    auto AddBody(const Massive &body) -> void {
        massiveBodies.insert(std::make_pair(body.GetId(), body));
        pastPoints.insert(std::make_pair(body.GetId(), vector<OrbitPoint>()));
        MassiveRender::AddBody(body);
    }

    auto AddBody(const Massless &body) -> void {
        masslessBodies.insert(std::make_pair(body.GetId(), body));
        pastPoints.insert(std::make_pair(body.GetId(), vector<OrbitPoint>()));
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

    auto GetPastPoints() -> unordered_map<string, vector<OrbitPoint>> {
        return pastPoints;
    }

    auto GetFuturePoints() -> unordered_map<string, vector<OrbitPoint>> {
        return futurePoints;
    }
}