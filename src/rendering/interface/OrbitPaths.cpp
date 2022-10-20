#include "OrbitPaths.h"

#include <rendering/geometry/Rays.h>
#include <util/Types.h>
#include <main/Bodies.h>



namespace OrbitPaths {

    namespace {
        auto TransformCoordinatesToScreenSpace(const unordered_map<string, vector<vec3>> &worldPositions) -> unordered_map<string, vector<vec2>> {
            unordered_map<string, vector<vec2>> screenPositions;
            for (const auto pair : worldPositions) {

                // Transform all positions for that bodyto screen space and add them to a vector
                vector<vec2> bodyScreenPositions = vector<vec2>();
                for (const vec3 worldPosition : pair.second) {
                    bodyScreenPositions.push_back(Rays::WorldToScreen(worldPosition));
                }

                // Assign the final vector to the body ID in the map
                screenPositions.insert(std::make_pair(pair.first, bodyScreenPositions));
            }
            return screenPositions;
        }
    }

    auto Init() -> void {
        
    }

    auto Update() -> void {
        // Find positions that each body will be in the future
        unordered_map<string, vector<vec3>> worldPositions = Bodies::GetPositions();

        // Loop through all the body IDs
        unordered_map<string, vector<vec2>> screenPositions = TransformCoordinatesToScreenSpace(worldPositions);

        // Remove positions outside of the screen

        // Remove positions occluded by massive bodies

        // Interpolate points that are too far apart

        // Draw the remaining points
    }
}