#pragma once

#include "../util/Types.h"



class Body {
protected:
    uvec3 position;
    uvec3 velocity;
    string name;

public:
    Body(uvec3 position, uvec3 velocity, string name);

    // returns vertices constituting an icon to represent the body
    vector<VERTEX_DATA_TYPE> GetIconVertices();
};
