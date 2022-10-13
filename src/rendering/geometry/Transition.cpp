#include "Transition.h"

#include <rendering/camera/Camera.h>

#include <glm/ext/quaternion_geometric.hpp>



const float Transition::A = 0.1643;
const float Transition::B = 1.9;

Transition::Transition(const vec3 positionStart, const vec3 positionEnd, const float zoomStart, const float zoomEnd, const float totalTime)
    : positionStart(positionStart),
      positionEnd(positionEnd),
      zoomStart(zoomStart),
      zoomEnd(zoomEnd),
      totalTime(totalTime),
      currentTime(0) {
        // If the zoom end is less than the zoom start there's no need to do any zooming
        // The purpose of this transition class is to be able to move between two bodies, and if
        // the user is sufficiently zoomed out, they won't clip inside the body, so we don't care
        // about zooming them in
        if (zoomEnd <= zoomStart) {
            this->zoomEnd = zoomStart;
        }
      }

auto Transition::Finished() const -> bool {
    return currentTime >= totalTime;
}


auto Transition::Step(const double deltaTime) -> void {
    // If the animation has finished, we can just return the point where the transition is supposed to end
    vec3 positionTarget = positionEnd;

    // Check that the animation hasn't been finished yet
    if (!Finished()) {

        // Increment time
        currentTime += float(deltaTime);

        // Find how far through the animation we are (in a range of 0-1)
        float x = currentTime / totalTime;

        // Substitute into the logistic equation
        float top = (x-0.5F) / A;                                   // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        float bottom = B * sqrtf(1 + powf((x-0.5F) / A, 2)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        float result = (top / bottom) + 0.5F;                       // NOLINT(cppcoreguidelines-avoid-magic-numbers)

        // Add the relevant vector in the direction of the animation to the starting point
        vec3 direction = positionEnd - positionStart;
        positionTarget = positionStart + (result * direction);

        // Add the relevant scalar to the zoom value
        float totalChangeInZoom = zoomEnd - zoomStart;
        float zoomTarget = zoomStart + (result * totalChangeInZoom);
        Camera::SetZoom(zoomTarget);
    }

    Camera::SetTarget(positionTarget);
}


auto Transition::UpdateTarget(const vec3 target) -> void {
    positionEnd = target;
}