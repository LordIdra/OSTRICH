#include "Rays.h"
#include "util/Constants.h"

#include <window/Window.h>
#include <rendering/camera/Camera.h>
#include <rendering/camera/Settings.h>
#include <util/Log.h>

#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>

#include <string>



namespace Rays {
    
    namespace {
        vec3 UP_DIRECTION = vec3(0, 1, 0);
    }

    auto Scale(const dvec3 coordinate) -> vec3 {
        return coordinate / SCALE_FACTOR;
    }

    auto Unscale(const vec3 coordinate) -> dvec3 {
        return dvec3(coordinate) * SCALE_FACTOR;
    }

    auto ScreenToWorld(const vec2 coords) -> vec3 {
        // Add w and z component
        vec4 coords1 = vec4(coords, -1.0, 1.0);

        // Transform to eye space
        vec4 coords2 = glm::inverse(Camera::GetProjection()) * coords1;

        // Set ray to point forwards and set w component to 0 to make this a vector
        vec4 coords3 = vec4(coords2.x, coords2.y, -1.0, 0.0);

        // Transform to world space
        vec4 coords4 = glm::inverse(Camera::GetView()) * coords3;

        // Remove w component to make this a vector
        vec3 coords5 = vec3(coords4.x, coords4.y, coords4.z);

        // Normalise vector
        vec3 coords6 = glm::normalize(coords5);
        
        return coords6;
    }

    auto WorldToScreen(const vec3 coords) -> vec2 {
        // Get model matrix
        mat4 modelMatrix = glm::translate(mat4(1.0), coords);

        // Transform from world space to eye space
        vec4 coords2 = Camera::GetMatrix() * modelMatrix * vec4(0, 0, 0, 1);

        // Rasterize 3D coordinates to screen
        vec2 coords3 = vec2(coords2.x / coords2.z, coords2.y / coords2.z);

        return coords3;
    }

    auto UnNormalize(vec2 coords) -> vec2 {
        // Get from -1 <-> 1 to 0 <-> 1
        coords += vec2(1, 1);
        coords *= vec2(0.5, 0.5); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        coords *= vec2(Window::GetWidth(), Window::GetHeight());
        return coords;
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

    auto RadiusOnScreen(const Massive &massive) -> double {
        // Find the vector pointing from the camera to the body
        vec3 cameraToBodyDirection = Camera::GetPosition() - massive.GetScaledPosition();

        // Find a unit vector perpendicular to cameraToBodyDirection
        vec3 perpendicularVector = massive.GetScaledRadius() * glm::normalize(glm::cross(UP_DIRECTION, cameraToBodyDirection));

        // Get the two relevant positions
        vec3 worldBodyPosition = massive.GetScaledPosition();
        vec3 worldEdgePosition = worldBodyPosition + perpendicularVector;

        // Convert the positions
        vec2 screenBodyPosition = WorldToScreen(worldBodyPosition);
        vec2 screenEdgePosition = WorldToScreen(worldEdgePosition);

        // Get the distance between them
        return glm::distance(screenBodyPosition, screenEdgePosition);
    }

    auto IsCoordinateOffCamera(const vec3 &coordinate) -> bool {
        vec3 cameraDirection = glm::normalize(Camera::GetPosition() - Camera::GetTarget());
        vec3 coordinateDirection = glm::normalize(Camera::GetPosition() - coordinate);
        double angle = acos(glm::dot(cameraDirection, coordinateDirection));
        return glm::degrees(angle) > 80.0;
    }

    auto IsCoordinateOffCamera(const vec2 &coordinate) -> bool {
        return (coordinate.x < -1) || (coordinate.x > 1) || (coordinate.y < -1) || (coordinate.y > 1);
    }
}