#pragma once

#include "../util/Types.h"
#include "../util/Constants.h"



namespace geometry {
    auto Sphere(vec3 position, float radius, float steps) -> vector<VERTEX_DATA_TYPE>;
}