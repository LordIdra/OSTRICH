#include "Camera.h"

#include <rendering/camera/Settings.h>
#include <rendering/camera/Util.h>
#include <window/Window.h>

#include <util/Constants.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>



namespace Camera {
    namespace {
        vec3 position(0, 0, 0);
        vec3 target(0, 0, 0);

        vec2 angle(0, 0);       // (horizontal, vertical)
        vec2 angleDelta(0, 0);  // (horizontal, vertical)

        float zoom = STARTING_ZOOM;
        float zoomDelta = 0;

        auto UpdatePosition() -> void {
            // First calculate the position of the camera
            // This can be calculated from the camera target, the zoom level, and the angle
            // angle.x means the horizontal plane, while angle.y means the vertical plane
            position.x = target.x + (zoom * cosf(angle.x) * cosf(angle.y));
            position.y = target.y + (zoom * sinf(angle.y));
            position.z = target.z + (zoom * sinf(angle.x) * cosf(angle.y));
        }
    }

    auto Init() -> void {
        Camera::SetTarget(vec3(0.0F, 0.0F, 0.0F));
    }

    auto Update(float minZoom) -> void {
        // Apply deltas
        angle[0] += angleDelta[0];
        angle[1] += angleDelta[1];
        zoom *= (1 - zoomDelta);

        // Damp the angle/zoom deltas
        ApplyDamping(angleDelta[0], ANGLE_DAMPING);
        ApplyDamping(angleDelta[1], ANGLE_DAMPING);
        ApplyDamping(zoomDelta, ZOOM_DAMPING);

        // Clamp the deltas to their minimums/maximums
        ApplySymmetricRange(angleDelta[0], MAX_ANGLE_SPEED);
        ApplySymmetricRange(angleDelta[1], MAX_ANGLE_SPEED);
        ApplySymmetricRange(zoomDelta, MAX_ZOOM_SPEED);

        // Clamp the angle and zoom to their minimums/maximums
        ApplySymmetricRange(angle[1], MAX_ANGLE);
        ApplyAsymmetricRange(zoom, minZoom, MAX_ZOOM);
    }

    auto GetView() -> mat4 {
        UpdatePosition();
        return glm::lookAt(position, target, VERTICAL);
    }

    auto GetProjection() -> mat4 {
        // Calculate aspect ratio (ratio of width to height)
        const float aspectRatio = float(Window::GetWidth()) / float(Window::GetHeight());

        // Calculate a perspective matrix
        return glm::perspective(glm::radians(FIELD_OF_VIEW), aspectRatio, NEAR_PLANE, FAR_PLANE);
    }

    auto GetMatrix() -> mat4 {        
        return GetProjection() * GetView();
    }

    auto GetPosition() -> vec3 {
        UpdatePosition();
        return position;
    }

    auto GetTarget() -> vec3 {
        return target;
    }

    auto GetZoom() -> float {
        return zoom;
    }

    auto AddAngleDelta(vec2 angleDelta_) -> void {
        angleDelta += angleDelta_ * ANGLE_SENSITIVITY;
    }

    auto AddZoomDelta(float zoomDelta_) -> void {
        zoomDelta += zoomDelta_ * ZOOM_SENSITIVITY;
    }

    auto SetTarget(vec3 target_) -> void {
        target = target_;
    }

    auto SetZoom(float zoom_) -> void {
        zoom = zoom_;
    }
}
