#pragma once

#include "Body.h"



class Massive : Body {
private:
    bfloat mass;   // kg
    bfloat radius; // m
    vec3 color;

    float ScaledRadius();

public:
    Massive(bvec3 position, bvec3 velocity, string name, bfloat mass, bfloat radius, vec3 color);

    // returns vertices constituting a sphere to represent the body
    vector<VERTEX_DATA_TYPE> GetSphereVertices();
};
