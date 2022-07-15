#include "geometry.h"

#include "../util/logging.h"



namespace geometry {

    const vector<VERTEX_DATA_TYPE> unitTetrahedron = {
        // TRIANGLE 1
        sqrtf(8.0/9.0),  0,                  -(1.0/3.0),// vertex
        sqrtf(8.0/9.0),  0,                  -(1.0/3.0),// normal

        -sqrtf(2.0/9.0),  sqrtf(2.0/3.0), -(1.0/3.0), // vertex
        -sqrtf(2.0/9.0),  sqrtf(2.0/3.0), -(1.0/3.0), // normal

        -sqrtf(2.0/9.0), -sqrtf(2.0/3.0), -(1.0/3.0), // vertex
        -sqrtf(2.0/9.0), -sqrtf(2.0/3.0), -(1.0/3.0), // normal

        // TRIANGLE 2
        sqrtf(8.0/9.0),  0,                  -(1.0/3.0), // vertex
        sqrtf(8.0/9.0),  0,                  -(1.0/3.0), // normal

        -sqrtf(2.0/9.0),  sqrtf(2.0/3.0), -(1.0/3.0), // vertex
        -sqrtf(2.0/9.0),  sqrtf(2.0/3.0), -(1.0/3.0), // normal

        0,                  0,                 1,          // vertex
        0,                  0,                 1,          // normal

        // TRIANGLE 3
        -sqrtf(2.0/9.0),  sqrtf(2.0/3.0), -(1.0/3.0), // vertex
        -sqrtf(2.0/9.0),  sqrtf(2.0/3.0), -(1.0/3.0), // normal

        -sqrtf(2.0/9.0), -sqrtf(2.0/3.0), -(1.0/3.0), // vertex
        -sqrtf(2.0/9.0), -sqrtf(2.0/3.0), -(1.0/3.0), // normal

        0,                  0,                 1,          // vertex
        0,                  0,                 1,          // normal

        // TRIANGLE 4
        sqrtf(8.0/9.0),  0,                  -(1.0/3.0), // vertex
        sqrtf(8.0/9.0),  0,                  -(1.0/3.0), // normal

        -sqrtf(2.0/9.0), -sqrtf(2.0/3.0), -(1.0/3.0),  // vertex
        -sqrtf(2.0/9.0), -sqrtf(2.0/3.0), -(1.0/3.0),  // normal

        0,                  0,                 1,           // vertex
        0,                  0,                 1,           // normal
    };

    namespace {
        auto PolarToCartesian(const float radius, const float phi, const float theta) -> vec3 {
            return vec3(
                radius * sinf(phi) * cosf(theta),
                radius * sinf(phi) * sinf(theta),
                radius * cosf(phi));
        }

        auto AddVertex(vector<VERTEX_DATA_TYPE> &sphere, const vec3 vertex) -> void{
            // here we use the elegant fact that the normal to a vertex on a sphere is
            // simply the unit vector from the centre of the sphere to the vertex
            vec3 normal = glm::normalize(vertex);

            sphere.push_back(vertex.x);
            sphere.push_back(vertex.y);
            sphere.push_back(vertex.z);

            sphere.push_back(normal.x);
            sphere.push_back(normal.y);
            sphere.push_back(normal.z);
        }

        auto GenerateTriangles(vector<VERTEX_DATA_TYPE> &sphere, vec3 position, float radius, float step, float phi, float theta1, float theta2) -> void {
            AddVertex(sphere, position + PolarToCartesian(radius, phi, theta1));
            AddVertex(sphere, position + PolarToCartesian(radius, phi + step, theta2));
            AddVertex(sphere, position + PolarToCartesian(radius, phi, theta1 + step));

            AddVertex(sphere, position + PolarToCartesian(radius, phi, theta1 + step));
            AddVertex(sphere, position + PolarToCartesian(radius, phi + step, theta2));
            AddVertex(sphere, position + PolarToCartesian(radius, phi + step, theta2 + step));
        }

    }

    auto Sphere(vec3 position, float radius, float steps) -> vector<VERTEX_DATA_TYPE> {
        vector<VERTEX_DATA_TYPE> sphere;
        float phi = 0;

        for (float i = 0; i < (PI - steps); i += 2*steps) {

            // first set of triangles
            float theta1 = 0;
            float theta2 = steps / 2;
            for (float j = 0; j < (2*PI); j += steps) {
                GenerateTriangles(sphere, position, radius, steps, phi, theta1, theta2);
                theta1 += steps;
                theta2 += steps;
            }

            phi += steps;

            // first set of triangles
            theta1 = steps / 2;
            theta2 = 0;
            for (float j = 0; j < (2*PI); j += steps) {
                GenerateTriangles(sphere, position, radius, steps, phi, theta1, theta2);
                theta1 += steps;
                theta2 += steps;
            }

            phi += steps;
        }

        return sphere;
    }
}

