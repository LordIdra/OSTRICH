#pragma once

#include <util/Types.h>



class Transition {
private:

    static const float A;
    static const float B;

    vec3 start;
    vec3 end;

    float totalTime;
    float currentTime;


public:
    Transition(const vec3 start, const vec3 end, const float totalTime);

    auto Finished() const -> bool;
    auto Step(const double deltaTime) -> vec3;
    auto UpdateTarget(const vec3 target) -> void;
};
