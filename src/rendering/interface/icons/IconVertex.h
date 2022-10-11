#pragma once

#include "util/Types.h"



struct IconVertex {
    float x; 
    float y;

    float r;
    float g;
    float b;

    IconVertex(float x, float y, vec3 color)
        : x(x), y(y), r(color.r), g(color.g), b(color.b) {}
};