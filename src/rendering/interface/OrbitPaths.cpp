#include "OrbitPaths.h"
#include "main/OrbitPoint.h"
#include "main/Simulation.h"
#include "rendering/VAO.h"
#include "util/Constants.h"
#include "util/Log.h"

#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <rendering/shaders/Program.h>
#include <rendering/geometry/Rays.h>
#include <unordered_map>
#include <util/Types.h>
#include <main/Bodies.h>

#include <glad/glad.h>

using std::unique_ptr;



namespace OrbitPaths {

    namespace {

        const int STRIDE = 5;
        const float PATH_WIDTH = 0.001;
        const vec3 PATH_COLOR = vec3(0.0, 0.0, 0.9);
        const float DISTANCE_THRESHOLD = 0.05;

        unique_ptr<VAO> vao;
        unique_ptr<Program> program;

        auto InterpolatePoints(unordered_map<string, vector<OrbitPoint>> &unscaledOrbitPoints) -> void {
            for (auto &pair : unscaledOrbitPoints) {
                for (int i = 1; i < pair.second.size()-1; i++) {

                    // Find how close this point and the next point are in screen space
                    float distance = glm::distance(Rays::Scale(pair.second.at(i).position), Rays::Scale(pair.second.at(i-1).position));

                    // If the points are close enough, interpolate them to create a new point in-between
                    // We also don't increment i because we want to check if this newly created point is sufficiently close to the previous one
                    if (distance > DISTANCE_THRESHOLD) {
                        pair.second.insert(pair.second.begin() + i  , Simulation::Integrate(Bodies::GetMassiveBodies(), pair.first, pair.second.at(i).timeToNextPoint / 2.0, pair.second.at(i)));
                        continue;
                    }
                }
            }
        }

        auto TransformCoordinatesToScreenSpace(unordered_map<string, vector<OrbitPoint>> &unscaledOrbitPoints) -> unordered_map<string, vector<vec2>> {
            // Transform all positions for that body to screen space and put them into a 'sequence'
            unordered_map<string, vector<vec2>> positionMap;
            for (const auto pair : unscaledOrbitPoints) {
                vector<vec2> positions;
                for (const OrbitPoint worldPositions : pair.second) {

                    // Check that the coordinate isn't actually BEHIND the camera
                    // If we don't check this now, weird stuff will happen when we attempt
                    // to map the coordinate to screen space, and it may actually end up on-screen in an unintended way
                    if (!Rays::IsCoordinateOffCamera(Rays::Scale(worldPositions.position))) {
                        positions.push_back(Rays::WorldToScreen(Rays::Scale(worldPositions.position)));
                    }
                }

                // Add the sequence to the list of sequences, then add the list of sequences to the position map
                // If the sequence can't be drawn (ie: less than 2 points can be drawn the camera), no need to add it
                if (positions.size() >= 2) {
                    positionMap.insert(std::make_pair(pair.first, positions));
                }
            }

            return positionMap;
        }

        auto RemoveOffScreenCoordinates(unordered_map<string, vector<vec2>> &unscaledPositionMap) -> unordered_map<string, vector<vector<vec2>>> {
            unordered_map<string, vector<vector<vec2>>> sequenceMap;
            for (auto pair : unscaledPositionMap) {

                // These sequences are series of points that need to be connected with a line
                // Why not just a continuous set of points? Well, part the orbit may go off camera, meaning it would be split
                // in two and we'd need to render two separate segments
                vector<vector<vec2>> bodyScreenPositions = vector<vector<vec2>>();
                for (int i = 1; i < pair.second.size()-1;) {
                    
                    // If the coordinate is off camera, no reason to create a sequence
                    if (Rays::IsCoordinateOffCamera(pair.second.at(i))) {
                        i++;
                        continue;
                    }

                    // Okay, we just found a coordinate that's on camera, let's start creating a sequence
                    vector<vec2> sequence;

                    // Add start of sequence - the first node might be on screen, so it'll look like the sequence
                    // starts on-screen even if it may start off-screen in actuality
                    sequence.push_back(pair.second.at(i-1));

                    // Keep adding points to the sequence until the point is off-camera
                    while ((i < pair.second.size()-1) && (!Rays::IsCoordinateOffCamera(pair.second.at(i)))) {
                        sequence.push_back(pair.second.at(i));
                        i++;
                    }

                    // Now close off the sequence by adding the next point
                    // This is done to make sure that lines actually appear to go off-screen, rather than
                    // just terminating near the end of the screen
                    sequence.push_back(pair.second.at(i));

                    // All done
                    bodyScreenPositions.push_back(sequence);
                    i++;
                }

                // Assign the final vector to the body ID in the map
                sequenceMap.insert(std::make_pair(pair.first, bodyScreenPositions));
            }

            return sequenceMap;
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
        unordered_map<string, vector<OrbitPoint>> unscaledPositionMap = Bodies::GetPositions();
        unordered_map<string, vector<vec2>> positionMap = TransformCoordinatesToScreenSpace(unscaledPositionMap);
        unordered_map<string, vector<vector<vec2>>> sequenceMap = RemoveOffScreenCoordinates(positionMap);

        // Interpolate points that are too far apart
        //InterpolatePoints(points);

        // Now we have a list of sequences, let's draw them
        // Iterate through every body
        vector<VERTEX_DATA_TYPE> vertices;
        for (const auto &pair : sequenceMap) {
            
            // Iterate through every sequence for the corresponding body
            for (const auto &sequence : pair.second) {

                // This would cause a segmentation fault, we need at least 2 points to draw a sequence
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
                for (int i = 0; i < sequence.size(); i++) {

                    // Set first pair of vertices to be the vertices from the previous node
                    v1 = v3;
                    v2 = v4;

                    vec2 direction;

                    // If there's both a previous and next node, find the direction to each of them, and then average it
                    if ((i != 0) && (i != (sequence.size() - 1))) {
                        vec2 unitVectorToPrevious = glm::normalize(sequence[i] - sequence[i-1]);
                        vec2 unitVectorToNext = glm::normalize(sequence[i] - sequence[i+1]);
                        direction = glm::normalize((unitVectorToPrevious + unitVectorToNext) / 2.0F);
                    }

                    // If there's a next node but no previous node, find the perpendicular direction of the vector
                    // from the current node to the next node by swapping the x and y of said vector
                    else if (i == 0) {
                        vec2 unitVectorToNext = glm::normalize(sequence[i] - sequence[i+1]);
                        direction = vec2(unitVectorToNext.y, unitVectorToNext.x);
                    }

                    // And the same for if there's a previous node but no next node
                    else if (i == (sequence.size() - 1)) {
                        vec2 unitVectorToPrevious = glm::normalize(sequence[i] - sequence[i-1]);
                        direction = vec2(unitVectorToPrevious.y, unitVectorToPrevious.x);
                    }

                    // Compute new vertices
                    v3 = sequence[i] + (direction * PATH_WIDTH);
                    v4 = sequence[i] - (direction * PATH_WIDTH);

                    // Use color of the body for color of orbit path
                    vec3 color = Bodies::GetBody(pair.first).GetColor();

                    // Add new vertices (two triangles to form a quadrilateral)
                    AddVertex(vertices, v1, color);
                    AddVertex(vertices, v2, color);
                    AddVertex(vertices, v3, color);

                    AddVertex(vertices, v2, color);
                    AddVertex(vertices, v3, color);
                    AddVertex(vertices, v4, color);
                }
            }
        }

        program->Use();
        vao->Data(vertices, vertices.size() / STRIDE, GL_DYNAMIC_DRAW);
        vao->Render();
    }
}