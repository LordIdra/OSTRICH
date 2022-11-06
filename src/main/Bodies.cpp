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
            // Move body to next point
            body.SetPosition(futurePoints.at(body.GetId()).at(1).position);
            body.SetVelocity(futurePoints.at(body.GetId()).at(1).velocity);

            // The point we just moved to is now also a past point 
            // If we used the previous point, there'd be a gap in the line before the body
            pastPoints.at(body.GetId()).push_back(futurePoints.at(body.GetId()).at(1));
            futurePoints.at(body.GetId()).erase(futurePoints.at(body.GetId()).begin());

            // Check that we don't have too many past points now
            if (pastPoints.at(body.GetId()).size() > MAX_PAST_POINTS) {
                pastPoints.at(body.GetId()).erase(pastPoints.at(body.GetId()).begin());
            }
        }

        auto IncrementBodyOrbitPoint() -> void {
            // Move the body to next point if necessary
            while (timeSinceLastOrbitPointUpdate >= Simulation::GetTimeStepSize()) {
                timeSinceLastOrbitPointUpdate -= Simulation::GetTimeStepSize();

                for (auto &pair : massiveBodies)  { UpdateBody(pair.second); }
                for (auto &pair : masslessBodies) { UpdateBody(pair.second); }
            }
        }
    }

    auto Init() -> void {
        // Initial camera lock
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

        // Update transition target,so that the camera follows the target
        if (selectedType == BODY_TYPE_MASSIVE) {
            MassiveRender::UpdateTransitionTarget(massiveBodies.at(selected));
        } else if (selectedType == BODY_TYPE_MASSLESS) {
            MassiveRender::UpdateTransitionTarget(masslessBodies.at(selected));
        }

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
                    selectedType = BODY_TYPE_MASSIVE;
                    selected = pair.first;
                    MassiveRender::StartTransition(pair.second);
            }
        }
    }

    auto AddBody(const Massive &body) -> void {
        massiveBodies.insert(std::make_pair(body.GetId(), body));
        pastPoints.insert(std::make_pair(body.GetId(), vector<OrbitPoint>()));
        MassiveRender::AddBody(body);
    }

    auto AddBody(const Massless &body) -> void {
        // Massless bodies only need icons, so we don't need to worry about the Render namespace
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

    auto GetPastPoints() -> unordered_map<string, vector<OrbitPoint>> {
        return pastPoints;
    }

    auto GetFuturePoints() -> unordered_map<string, vector<OrbitPoint>> {
        return futurePoints;
    }
}