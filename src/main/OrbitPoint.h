#pragma once

#include <util/Types.h>



struct OrbitPoint {
    dvec3 position;
    dvec3 velocity;
    double timeToNextPoint;
};