#pragma once

#include <util/Types.h>



class Body {
protected:
    bvec3 position; // m
    bvec3 velocity; // m/s
    string name;

public:
    Body(bvec3 position, bvec3 velocity, string name);

    auto GetName() -> string;
    vec3 ScaledPosition();
    vec3 ScaledVelocity();

    // returns vertices constituting an icon to represent the body
    vector<VERTEX_DATA_TYPE> GetIconVertices();
};
