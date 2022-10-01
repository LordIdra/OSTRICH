#include "Interface.h"
#include "rendering/camera/Camera.h"

#include <glm/gtx/dual_quaternion.hpp>
#include <glm/trigonometric.hpp>
#include <util/Log.h>
#include <glm/gtx/string_cast.hpp>

#include <main/Bodies.h>
#include <rendering/camera/Settings.h>
#include <rendering/geometry/Rays.h>



namespace Interface {

    namespace {
        const double RADIUS_THRESHOLD = 0.01;
    }

    auto DrawIcons() -> void {
        for (const auto pair : Bodies::GetMassiveBodies()) {

            // World and screen coordinates of the body
            vec3 worldCoords = pair.second.GetScaledPosition();
            vec2 screenCoords = Rays::WorldToScreen(worldCoords);

            // Calculatee the angle between the camera direction and the vector from the camera to the body
            vec3 cameraDirection = glm::normalize(Camera::GetPosition() - Camera::GetTarget());
            vec3 bodyDirection = glm::normalize(Camera::GetPosition() - pair.second.GetScaledPosition());
            double angle = acos(glm::dot(cameraDirection, bodyDirection));

            // Find the radius of the circle drawn to represent the sphere on-screen
            double sphereRadiusOnScreen = Rays::RadiusOnScreen(pair.second);

            // If the body is off-screen, skip it
            if (screenCoords.x < -1.1 || screenCoords.x > 1.1 || screenCoords.y < -1.1 || screenCoords.y > 1.1) {
                continue;
            }

            // If the body is off-camera, skip it           
            if (glm::degrees(angle) > FIELD_OF_VIEW) {
                continue;
            }

            // If the body is too large a radius, skip it
            if (sphereRadiusOnScreen > RADIUS_THRESHOLD) {
                continue;
            }

            Log(INFO, pair.first);
        }

        Log(INFO, "");
    }
}