#pragma once

#include "Body.h"



class Massive : public Body {
private:
    bfloat mass;   // kg
    bfloat radius; // m

public:
    Massive(bvec3 position, bvec3 velocity, string name, bfloat mass, bfloat radius);

    float ScaledRadius();
    
    // returns vertices constituting a sphere to represent the body
    vector<VERTEX_DATA_TYPE> GetSphereVertices();
};
