#include "CameraTransition.h"

#include <rendering/camera/Camera.h>
#include <rendering/geometry/Rays.h>
#include <rendering/geometry/Transition.h>
#include <rendering/world/Icons.h>
#include <main/Bodies.h>
#include <input/Mouse.h>



namespace CameraTransition {
    namespace {
        const float TRANSITION_TIME = 0.2;
        const vec3 ZERO_VECTOR = vec3(0, 0, 0);
        
        Transition transition = Transition(ZERO_VECTOR, ZERO_VECTOR, 0.0, 0.0, 0.0);

        auto StartTransition(const Body &body) -> void {
            transition = Transition(
                Camera::GetTarget(),
                body.GetScaledPosition(), 
                Camera::GetZoom(),
                Bodies::GetMinZoom(),
                TRANSITION_TIME);
        }

        auto SwitchBodyBasedOnIcon() -> void {
            for (const auto &pair : Icons::GenerateIcons()) {
                if (pair.second.MouseOnIcon(Icon::SELECT_THRESHOLD)) {
                    SetTargetBody(pair.first);
                }
            }
        }

        auto SwitchSelectedBodyWhenSphereClicked() -> void {
            // This function only cares about the spheres representing massive bodies
            // Seletcing via icons is handles in the Icons namespace
            // Find the camera direction
            vec3 direction = Rays::ScreenToWorld(Mouse::GetScreenPosition());

            // Loop through every body
            for (auto &pair : Bodies::GetMassiveBodies()) {

                // Check if the camera direction intersects the body
                if (Rays::IntersectsSphere(
                    Camera::GetPosition(), 
                    direction, 
                    pair.second.GetScaledPosition(), 
                    pair.second.GetScaledRadius())) {

                        // If so, update the selected body and start a transition
                        SetTargetBody(pair.first);
                }
            }
        }
    }

    auto Init() -> void {
        Mouse::SetCallbackLeftDouble(SwitchSelectedBodyWhenSphereClicked);
        Mouse::SetCallbackLeftDouble(SwitchBodyBasedOnIcon);
    }

    auto Reset() -> void {
        transition = Transition(ZERO_VECTOR, ZERO_VECTOR, 0.0, 0.0, 0.0);
    }

    auto Update(const double deltaTime) -> void {
        // Update transition target, so that the camera follows the target
        if (Bodies::GetSelectedType() == BODY_TYPE_MASSIVE) {
            transition.UpdateTarget(Bodies::GetMassiveBody(Bodies::GetSelectedBody()).GetScaledPosition());
        } else if (Bodies::GetSelectedType() == BODY_TYPE_MASSLESS) {
            transition.UpdateTarget(Bodies::GetMasslessBody(Bodies::GetSelectedBody()).GetScaledPosition());
        }

        // Step transition
        transition.Step(deltaTime);
    }

    auto SetTargetBody(const string &id) -> void {
        Bodies::SetSelectedBody(id);
        StartTransition(Bodies::GetBody(id));
    }
}