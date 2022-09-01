#include "Transition.h"

#include <util/Log.h>

#include <glm/ext/quaternion_geometric.hpp>



const float Transition::A = 0.1643;
const float Transition::B = 1.9;

Transition::Transition(const vec3 start, const vec3 end, const float totalTime)
    : start(start),
      end(end),
      totalTime(totalTime),
      currentTime(0) {}

auto Transition::Finished() const -> bool {
    return currentTime >= totalTime;
}


auto Transition::Step(const float deltaTime) -> vec3 {
    // Check that the animation hasn't been finished yet
    if (!Finished()) {

        // Increment time
        currentTime += deltaTime;

        // Find how far through the animation we are (in a range of 0-1)
        float x = currentTime / totalTime;

        // Substitute into the logistic equation
        float top = (x-0.5F) / A;                                   // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        float bottom = B * sqrtf(1 + powf((x-0.5F) / A, 2)); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        float result = (top / bottom) + 0.5F;                       // NOLINT(cppcoreguidelines-avoid-magic-numbers)

        // Add the relevant vector in the direction of the animation to the starting point
        vec3 direction = end - start;
        return start + (result * direction);
    }

    // If the animation has finished, we can just return the point where the transition is supposed to end
    return end;
}