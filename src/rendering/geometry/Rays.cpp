#include "Rays.h"

#include <window/Window.h>
#include <rendering/camera/Camera.h>
#include <util/Log.h>

#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>

#include <string>



namespace Rays {
    auto ScreenToWorld(const vec2 coords) -> vec3 {
        // Invert y coordinates, so they start from the bottom left rather than top left
        vec2 coords1 = vec2(coords.x, 1 - coords.y);

        // Linearise to the range -1 to 1
        vec2 coords2 = vec2(coords1.x*2 - 1, coords1.y*2 - 1);

        // Add w and z component
        vec4 coords3 = vec4(coords2, -1.0, 1.0);

        // Transform to eye space
        vec4 coords4 = glm::inverse(Camera::GetProjection()) * coords3;

        // Set ray to point forwards and set w component to 0 to make this a vector
        vec4 coords5 = vec4(coords4.x, coords4.y, -1.0, 0.0);

        // Transform to world soace
        vec4 coords6 = glm::inverse(Camera::GetView()) * coords5;

        // Remove w component to make this a vector
        vec3 coords7 = vec3(coords6.x, coords6.y, coords6.z);

        // Normalise vector
        vec3 coords8 = glm::normalize(coords7);
        
        return coords8;
    }

    auto IntersectsSphere(const vec3 cameraPosition, const vec3 cameraDirection, const vec3 spherePosition, const float sphereRadius) -> bool {
        // Calculate lambda; a variable that can be multiplied by cameraDirection to obtain a vector from cameraPosition to the 
        // closest point the ray gets to spherePosition
        const float lambda = -glm::dot(cameraPosition - spherePosition, cameraDirection);

        // If lambda is below zero, the ray's closest intersection is behind the camera, so we don't want to select that
        if (lambda < 0) {
            return false;
        }

        // Find the closest point to the camera, the find the distance between that point and the sphere centre
        vec3 closestPoint = cameraPosition + (lambda*cameraDirection);
        const float distance = glm::distance(closestPoint, spherePosition);

        // If that distance is under the sphere radius, the ray must intersect the sphere
        return distance <= sphereRadius;
    }
}