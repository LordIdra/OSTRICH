#include "Sphere.h"

#include <util/Log.h>
#include <util/Constants.h>



namespace Sphere {

    namespace {
        auto PolarToCartesian(const float radius, const float phi, const float theta) -> vec3 {
            // Phi representsd the horizontal component while theta represents the vertical component
            return vec3(
                radius * sinf(phi) * cosf(theta),
                radius * sinf(phi) * sinf(theta),
                radius * cosf(phi));
        }

        auto AddVertex(vector<VERTEX_DATA_TYPE> &vertices, const vec3 vertex) -> void{
            // Here we use the elegant fact that the normal to a vertex on a sphere is
            // simply the unit vector from the centre of the sphere to the vertex
            vec3 normal = glm::normalize(vertex);

            // Add positions
            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);

            // Add normals
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
        }

        auto GenerateTriangles(vector<VERTEX_DATA_TYPE> &vertices, const float radius, const float step, const float phi, const float theta1, const float theta2) -> void {
            // First triangle
            AddVertex(vertices, PolarToCartesian(radius, phi, theta1));
            AddVertex(vertices, PolarToCartesian(radius, phi + step, theta2));
            AddVertex(vertices, PolarToCartesian(radius, phi, theta1 + step));

            // Second triangle
            AddVertex(vertices, PolarToCartesian(radius, phi, theta1 + step));
            AddVertex(vertices, PolarToCartesian(radius, phi + step, theta2));
            AddVertex(vertices, PolarToCartesian(radius, phi + step, theta2 + step));
        }

        auto GenerateTriangleSet(vector<VERTEX_DATA_TYPE> &vertices, const float radius, const float step, const float phi, float theta1,  float theta2) -> void {
            // Iterate vertically
            for (int j = 0; j < int(2*PI/step); j++) {
                GenerateTriangles(vertices, radius, step, phi, theta1, theta2);
                theta1 += step;
                theta2 += step;
            }
        }
    }

    auto Sphere(const float radius, const float step) -> vector<VERTEX_DATA_TYPE> {
        // Phi is the horizontal component of the angle of the sphere
        vector<VERTEX_DATA_TYPE> vertices;
        float phi = 0;

        // Iterate horizontally
        for (int i = 0; i < int((PI) / (2*step)); i++) {
            
            // First set of triangles
            GenerateTriangleSet(vertices, radius, step, phi, 0, step / 2);
            phi += step;

            // Second set of triangles
            GenerateTriangleSet(vertices, radius, step, phi, step / 2, 0);
            phi += step;
        }

        return vertices;
    }
}

