#include "OrbitPaths.h"
#include "rendering/VAO.h"
#include "util/Constants.h"
#include "util/Log.h"

#include <glm/geometric.hpp>
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
        const float PATH_WIDTH = 0.001;
        const vec3 PATH_COLOR = vec3(0.0, 0.0, 0.9);

        unique_ptr<VAO> vao;
        unique_ptr<Program> program;

        auto TransformCoordinatesToScreenSpace(const unordered_map<string, vector<dvec3>> &unscaledWorldPositions) -> unordered_map<string, vector<vector<vec2>>> {
            unordered_map<string, vector<vector<vec2>>> screenPositions;
            for (const auto pair : unscaledWorldPositions) {

                // Transform all positions for that bodyto screen space and add them to a vector
                vector<vector<vec2>> bodyScreenPositions = vector<vector<vec2>>();
                for (int i = 0; i < pair.second.size()-2;) {
                    if (Rays::IsCoordinateOffCamera(pair.second.at(i) / SCALE_FACTOR)) {
                        i++;
                        continue;
                    }
                    vector<vec2> sequence;
                    while ((i < pair.second.size()-2) && (!Rays::IsCoordinateOffCamera(pair.second.at(i) / SCALE_FACTOR))) {
                        sequence.push_back(Rays::WorldToScreen(pair.second.at(i) / SCALE_FACTOR));
                        i++;
                    }
                    sequence.push_back(pair.second.at(i+1));
                    bodyScreenPositions.push_back(sequence);
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
        unordered_map<string, vector<dvec3>> worldPositions = Bodies::GetPositions();

        // Loop through all the body IDs
        unordered_map<string, vector<vector<vec2>>> screenPositions = TransformCoordinatesToScreenSpace(worldPositions);

        // Remove positions outside of the screen
        //unordered_map<string, vector<vector<vec2>>> screenPositions1;
        //for (const auto &pair : screenPositions) {
        //    int i = 0;
        //    while (i < pair.second.size()-2) {
        //        vector<vec2> positionSequence;
        //        while (
        //            (pair.second.at(i+1).x > -1) && (pair.second.at(i+1).x < 1) && 
        //            (pair.second.at(i+1).y > -1) && (pair.second.at(i+1).x < 1) &&
        //            (i < pair.second.size()-2)) {
        //                Log(INFO, "bruh" + std::to_string(i));
        //            positionSequence.push_back(pair.second.at(i));
        //            i++;
        //        }
        //        if (i < pair.second.size()) {
        //            positionSequence.push_back(pair.second.at(i+1));
        //        }
        //    }
        //}
        //Log(INFO, "bro");

        // Remove positions occluded by massive bodies

        // Interpolate points that are too far apart

        // Draw the remaining points
        vector<VERTEX_DATA_TYPE> vertices;
        for (const auto &pair : screenPositions) {

            for (const auto &sequence : pair.second) {
                // This would cause a segmentation fault...
                if (sequence.size() < 2) {
                    continue;
                }
                vec2 v1;
                vec2 v2;
                vec2 v3 = sequence[0];
                vec2 v4 = sequence[0];
                for (int i = 1; i < sequence.size()-1; i++) {
                    v1 = v3;
                    v2 = v4;
                    vec2 unitVectorToPrevious = glm::normalize(sequence[i] - sequence[i-1]);
                    vec2 unitVectorToNext = glm::normalize(sequence[i] - sequence[i+1]);
                    vec2 direction = glm::normalize((unitVectorToPrevious + unitVectorToNext) / 2.0F);
                    v3 = sequence[i] + (direction * PATH_WIDTH);
                    v4 = sequence[i] - (direction * PATH_WIDTH);

                    vertices.push_back(v1.x);
                    vertices.push_back(v1.y);
                    vertices.push_back(PATH_COLOR.x);
                    vertices.push_back(PATH_COLOR.y);
                    vertices.push_back(PATH_COLOR.z);

                    vertices.push_back(v2.x);
                    vertices.push_back(v2.y);
                    vertices.push_back(PATH_COLOR.x);
                    vertices.push_back(PATH_COLOR.y);
                    vertices.push_back(PATH_COLOR.z);

                    vertices.push_back(v3.x);
                    vertices.push_back(v3.y);
                    vertices.push_back(PATH_COLOR.x);
                    vertices.push_back(PATH_COLOR.y);
                    vertices.push_back(PATH_COLOR.z);

                    vertices.push_back(v2.x);
                    vertices.push_back(v2.y);
                    vertices.push_back(PATH_COLOR.x);
                    vertices.push_back(PATH_COLOR.y);
                    vertices.push_back(PATH_COLOR.z);

                    vertices.push_back(v3.x);
                    vertices.push_back(v3.y);
                    vertices.push_back(PATH_COLOR.x);
                    vertices.push_back(PATH_COLOR.y);
                    vertices.push_back(PATH_COLOR.z);

                    vertices.push_back(v4.x);
                    vertices.push_back(v4.y);
                    vertices.push_back(PATH_COLOR.x);
                    vertices.push_back(PATH_COLOR.y);
                    vertices.push_back(PATH_COLOR.z);
                }
            }
        }

        program->Use();
        vao->Data(vertices, vertices.size() / STRIDE, GL_DYNAMIC_DRAW);
        vao->Render();
    }
}