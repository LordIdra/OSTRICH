#pragma once

#include <util/Types.h>



class Transition {
private:

    static const float A;
    static const float B;

    vec3 positionStart;
    vec3 positionEnd;

    float zoomStart;
    float zoomEnd;

    float totalTime;
    float currentTime;


public:
    Transition(const vec3 positionStart, const vec3 positionEnd, const float zoomStart, const float zoomEnd, const float totalTime);

    auto Finished() const -> bool;
    auto Step(const double deltaTime) -> void;
    auto UpdateTarget(const vec3 target) -> void;
};
