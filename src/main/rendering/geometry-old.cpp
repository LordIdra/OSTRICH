#include "geometry.h"



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
        auto Interpolate(const vec3 in1, const vec3 in2, const vec3 in3) -> vec3 {
            return (in1 + in2 + in3) / 3.0f;
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

        auto Split(vector<VERTEX_DATA_TYPE> inSphere, float radius) -> vector<VERTEX_DATA_TYPE> {
            vector<VERTEX_DATA_TYPE> sphere;
            for (int i = 0; i < inSphere.size(); i += 18) {

                // get existing vertices
                vec3 v1(inSphere[i], inSphere[i+1], inSphere[i+2]);
                vec3 v2(inSphere[i+6], inSphere[i+7], inSphere[i+8]);
                vec3 v3(inSphere[i+12], inSphere[i+13], inSphere[i+14]);

                // calculate new vertex
                vec3 v4 = radius * glm::normalize(Interpolate(v1, v2, v3));

                // create new triangles - this will recalculate normals automatically
                // technically we could omit the normals entirely until the last step
                // however, this would make the code significantly more complex

                AddVertex(sphere, v1);
                AddVertex(sphere, v2);
                AddVertex(sphere, v4);
                
                AddVertex(sphere, v1);
                AddVertex(sphere, v3);
                AddVertex(sphere, v4);

                AddVertex(sphere, v2);
                AddVertex(sphere, v3);
                AddVertex(sphere, v4);
            }

            return sphere;
        }
    }

    auto Sphere(float radius, float steps) -> vector<VERTEX_DATA_TYPE> {
        vector<VERTEX_DATA_TYPE> sphere = unitTetrahedron;

        // create a tetrahedron with the requested radius
        for (VERTEX_DATA_TYPE &vertex : sphere) {
            vertex *= radius;
        }

        // interpolate the tetrahedron 'steps' times
        for (int i = 0; i < steps; i++) {
            sphere = Split(sphere, radius);
        }

        return sphere;
    }
}

