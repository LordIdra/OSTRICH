#include "OrbitPaths.h"
#include "rendering/VAO.h"

#include <memory>
#include <rendering/shaders/Program.h>
#include <rendering/geometry/Rays.h>
#include <util/Types.h>
#include <main/Bodies.h>

#include <glad/glad.h>

using std::unique_ptr;



namespace OrbitPaths {

    namespace {

        const int STRIDE = 5;

        unique_ptr<VAO> vao;
        unique_ptr<Program> program;

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
        // Create Program
        Shader vertex = Shader("../resources/shaders/path-vertex.vsh", GL_VERTEX_SHADER);
        Shader fragment = Shader("../resources/shaders/path-fragment.fsh", GL_FRAGMENT_SHADER);
        program = std::make_unique<Program>(vertex, fragment);

        // Create VAO
        vao = std::make_unique<VAO>();
        vao->Init();
        vao->AddVertexAttribute(
            VertexAttribute{
            .index = 0,
            .size = 2,
            .type = GL_FLOAT,
            .normalised = GL_FALSE,
            .stride = STRIDE * sizeof(float),
            .offset = nullptr});
        vao->AddVertexAttribute(VertexAttribute{
            .index = 1,
            .size = 3,
            .type = GL_FLOAT,
            .normalised = GL_FALSE,
            .stride = STRIDE * sizeof(float),
            .offset = (void*)(2 * sizeof(float))}); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
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