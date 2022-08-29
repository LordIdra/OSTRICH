#include "Transition.h"

#include "Log.h"

#include <glm/ext/quaternion_geometric.hpp>



Transition::Transition(vec3 start, vec3 end, float totalTime)
    : start(start),
      end(end),
      totalTime(totalTime),
      currentTime(0) {}

vec3 Transition::Step(float deltaTime) {
    if (!Finished()) {
        currentTime += deltaTime;
        float x = currentTime / totalTime;
        float a = 0.1643;
        float b = 1.9;
        float top = (x-0.5) / a;
        float bottom = b * sqrt(1 + pow((x-0.5) / a, 2));
        float result = (top / bottom) + 0.5;
        return start + (result * (end - start));
    } else {
        return end;
    }
}

bool Transition::Finished() {
    return currentTime >= totalTime;
}
