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

                // Transform all positions for that body to screen space and create 'sequences'
                // These sequences are series of points that need to be connected with a line
                // Why not just a continuous set of points? Well, part the orbit may go off camera, meaning it would be split
                // in two and we'd need to render two separate segments
                vector<vector<vec2>> bodyScreenPositions = vector<vector<vec2>>();
                for (int i = 0; i < pair.second.size()-2;) {
                    
                    // If the coordinate is off camera, no reason to create a se
                    if (Rays::IsCoordinateOffCamera(pair.second.at(i) / SCALE_FACTOR)) {
                        i++;
                        continue;
                    }

                    // Okay, we just found a coordinate that's on camera, let's start creating a sequence
                    vector<vec2> sequence;

                    // Keep adding points to the sequence until the point is off-camera
                    while ((i < pair.second.size()-2) && (!Rays::IsCoordinateOffCamera(pair.second.at(i) / SCALE_FACTOR))) {
                        sequence.push_back(Rays::WorldToScreen(pair.second.at(i) / SCALE_FACTOR));
                        i++;
                    }

                    // Now close off the sequence by adding the next point
                    // This is done to make sure that lines actually appear to go off-screen, rather than
                    // just terminating near the end of the screen
                    sequence.push_back(pair.second.at(i+1));

                    // All done
                    bodyScreenPositions.push_back(sequence);
                }

                // Assign the final vector to the body ID in the map
                screenPositions.insert(std::make_pair(pair.first, bodyScreenPositions));
            }
            return screenPositions;
        }

        auto AddVertex(vector<VERTEX_DATA_TYPE> &vertices, const vec2 position, const vec3 color) -> void {
            vertices.push_back(position.x);
            vertices.push_back(position.y);
            vertices.push_back(color.x);
            vertices.push_back(color.y);
            vertices.push_back(color.z);
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

        // Now we have a list of sequences, let's draw them
        // Iterate through every body
        vector<VERTEX_DATA_TYPE> vertices;
        for (const auto &pair : screenPositions) {
            
            // Iterate through every sequence for the corresponding body
            for (const auto &sequence : pair.second) {

                // This would cause a segmentation fault, we need at least 3 points to draw a sequence
                // due to how we use the previous and next nodes to figure out the vertices for a node
                if (sequence.size() < 2) {
                    continue;
                }

                // Initialise vertices
                // v1/v2 are set in the loop body, v3/v4 can be set to the position of the node
                // Yes this creates a triangle instead of a quadrilateral, but A) the initial point is likely to be
                // hidden, and B) the paths are so thin that it's practically impossible to notice this
                vec2 v1;
                vec2 v2;
                vec2 v3 = sequence[0];
                vec2 v4 = sequence[0];

                // Loop through everything in the sequence, starting one element late and terminating one element early
                for (int i = 1; i < sequence.size()-1; i++) {

                    // Set first pair of vertices to be the vertices from the previous node
                    v1 = v3;
                    v2 = v4;

                    // Compute new vertices
                    vec2 unitVectorToPrevious = glm::normalize(sequence[i] - sequence[i-1]);
                    vec2 unitVectorToNext = glm::normalize(sequence[i] - sequence[i+1]);
                    vec2 direction = glm::normalize((unitVectorToPrevious + unitVectorToNext) / 2.0F);
                    v3 = sequence[i] + (direction * PATH_WIDTH);
                    v4 = sequence[i] - (direction * PATH_WIDTH);

                    // Add new vertices (two triangles to form a quadrilateral)
                    AddVertex(vertices, v1, PATH_COLOR);
                    AddVertex(vertices, v2, PATH_COLOR);
                    AddVertex(vertices, v3, PATH_COLOR);

                    AddVertex(vertices, v2, PATH_COLOR);
                    AddVertex(vertices, v3, PATH_COLOR);
                    AddVertex(vertices, v4, PATH_COLOR);
                }
            }
        }

        program->Use();
        vao->Data(vertices, vertices.size() / STRIDE, GL_DYNAMIC_DRAW);
        vao->Render();
    }
}