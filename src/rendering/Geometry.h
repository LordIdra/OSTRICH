#pragma once

#include "../util/Types.h"



namespace Geometry {
    auto Sphere(vec3 position, float radius, float steps) -> vector<VERTEX_DATA_TYPE>;
}