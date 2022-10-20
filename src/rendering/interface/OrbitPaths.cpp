#include "OrbitPaths.h"

#include <util/Types.h>
#include <main/Bodies.h>



namespace OrbitPaths {

    auto Draw() -> void {
        // Find positions that each body will be in the future
        unordered_map<string, vector<vec3>> positions = Bodies::GetPositions();

        // Transform all positions to screen space

        // Remove positions outside of the screen

        // Remove positions occluded by massive bodies

        // Interpolate points that are too far apart

        // Draw the remaining points
    }
}