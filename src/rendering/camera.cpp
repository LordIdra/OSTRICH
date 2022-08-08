#include "camera.h"

#include "../util/logging.h"
#include "../util/Constants.h"
#include "../window/window.h"

#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>



namespace camera {
    namespace {
        const float MIN_ZOOM = 0.002;
        const float MAX_ZOOM = 95.0;

        const float MAX_ANGLE = PI * 0.45;

        const float MAX_ANGLE_SPEED = 0.01;
        const float MAX_ZOOM_SPEED = 0.1;

        const float ANGLE_SENSITIVITY = 20.0;
        const float ZOOM_SENSITIVITY = 0.03;
        
        const float ANGLE_DAMPING = 0.001;
        const float ZOOM_DAMPING = 0.01;

        vec3 position(0, 0, 0);
        vec3 target(0, 0, 0);

        vec2 angle(0, 0); // (horizontal, vertical)
        vec2 angleDelta(0, 0);

        float zoom = 0.05;
        float zoomDelta = 0;

        auto ApplyDamping(float &x, float damping) -> void {
            // a small value is added to account for floating point errors
            const float threshold = (damping + 0.00001);
            if (x < -threshold) {
                x += damping;
            } else if (x > threshold) {
                x -= damping;
            } else {
                x = 0;
            }
        }

        auto ApplySymmetricRange(float &x, float range) -> void {
            // a small value is added to account for floating point errors
            const float threshold = (range + 0.00001);
            if (x < -threshold) {
                x = -threshold;
            } else if (x > threshold) {
                x = threshold;
            }
        }

        auto ApplyAsymmetricRange(float &x, float min, float max) -> void {
            if (x < min) {
                x = min;
            } else if (x > max) {
                x = max;
            }
        }
    }

    auto GetView() -> mat4 {
        // angle.x means the horizontal plane, while angle.y means the vertical plane
        position.y = target.y + (zoom * sinf(angle.y));
        position.x = target.x + (zoom * cosf(angle.x) * cosf(angle.y));
        position.z = target.z + (zoom * sinf(angle.x) * cosf(angle.y));

        return glm::lookAt(position, target, VERTICAL);
    }

    auto GetProjection() -> mat4 {
        const float aspectRatio = ((float)window::GetWidth()) / ((float)window::GetHeight());
        return glm::perspective(
            (float)glm::radians(45.0f),
            aspectRatio, 
            0.0001f,
            100.0f);
    }

    auto GetMatrix() -> mat4 {        
        return GetProjection() * GetView();
    }

    auto GetPosition() -> vec3 {
        // angle.x means the horizontal plane, while angle.y means the vertical plane
        position.y = target.y + (zoom * sinf(angle.y));
        position.x = target.x + (zoom * cosf(angle.x) * cosf(angle.y));
        position.z = target.z + (zoom * sinf(angle.x) * cosf(angle.y));
        
        return position;
    }

    auto GetTarget() -> vec3 {
        return target;
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

    auto Update() -> void {
        angle[0] += angleDelta[0];
        angle[1] += angleDelta[1];
        zoom *= (1 - zoomDelta);

        ApplyDamping(angleDelta[0], ANGLE_DAMPING);
        ApplyDamping(angleDelta[1], ANGLE_DAMPING);
        ApplyDamping(zoomDelta, ZOOM_DAMPING);

        ApplySymmetricRange(angleDelta[0], MAX_ANGLE_SPEED);
        ApplySymmetricRange(angleDelta[1], MAX_ANGLE_SPEED);
        ApplySymmetricRange(zoomDelta, MAX_ZOOM_SPEED);

        ApplySymmetricRange(angle[1], MAX_ANGLE);
        ApplyAsymmetricRange(zoom, MIN_ZOOM, MAX_ZOOM);
    }
}
