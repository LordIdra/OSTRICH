#include "Rays.h"

#include <window/Window.h>
#include <rendering/Camera.h>
#include <util/Log.h>

#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>

#include <string>



namespace Rays {
    auto ScreenToWorld(vec2 coords) -> vec3 {

        // invert y coordinates, so they start from the bottom left rather than top left
        coords.y = 1 - coords.y;

        // linearise to the range -1 to 1
        vec2 coords1 = vec2(coords.x*2 - 1, coords.y*2 - 1);

        // add w and z component
        vec4 coords2 = vec4(coords1, -1.0, 1.0);

        // transform to eye space
        vec4 coords3 = glm::inverse(Camera::GetProjection()) * coords2;

        // set ray to point forwards and set w component to 0 to make this a vector
        vec4 coords4 = vec4(coords3.x, coords3.y, -1.0, 0.0);

        // transform to world soace
        vec4 coords5 = glm::inverse(Camera::GetView()) * coords4;

        // remove w component to make this a vector
        vec3 coords6 = vec3(coords5.x, coords5.y, coords5.z);

        // normalise vector
        vec3 coords7 = glm::normalize(coords6);
        
        return coords7;
    }    

    auto IntersectsSphere(vec3 cameraPosition, vec3 cameraDirection, vec3 spherePosition, float sphereRadius) -> bool {
        float lambda = -glm::dot(cameraPosition - spherePosition, cameraDirection);
        if (lambda < 0) {
            return false;
        }
        vec3 closestPoint = cameraPosition + (lambda*cameraDirection);
        float distance = glm::distance(closestPoint, spherePosition);
        return distance <= sphereRadius;
    }
}