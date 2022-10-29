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
#include <string>
#include <unordered_map>
#include <util/Types.h>
#include <main/Bodies.h>

#include <glad/glad.h>

using std::unique_ptr;



namespace OrbitPaths {

    namespace {

        const int STRIDE = 5;
        const float PATH_WIDTH = 0.005;
        const vec3 PATH_COLOR = vec3(0.0, 0.0, 0.9);
        const float DISTANCE_THRESHOLD = 0.01;

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

        auto ConvertToSequences(unordered_map<string, vector<OrbitPoint>> &unscaledOrbitPoints) -> unordered_map<string, vector<vector<OrbitPoint>>> {
            unordered_map<string, vector<vector<OrbitPoint>>> sequenceMap;
            for (auto pair : unscaledOrbitPoints) {

                // These sequences are series of points that need to be connected with a line
                // Why not just a continuous set of points? Well, part the orbit may go off camera, meaning it would be split
                // in two and we'd need to render two separate segments
                vector<vector<OrbitPoint>> bodyScreenPositions = vector<vector<OrbitPoint>>();
                for (int i = 0; i < pair.second.size()-1;) {
                    
                    // If the coordinate is off camera, no reason to create a sequence
                    // Note that here we do checks. One check takes a vec3 and makes sure that the coordinate isn't actually BEHIND the camera
                    // The second check takes a vec2 and makes sure it isn't off-screen
                    vec3 scaledCoordinate = Rays::Scale(pair.second.at(i).position);
                    bool isCoordinateBehindCamera = Rays::IsCoordinateOffCamera(scaledCoordinate);
                    bool isCoordinateOffScreen = Rays::IsCoordinateOffCamera(Rays::WorldToScreen(scaledCoordinate));
                    if (isCoordinateOffScreen || isCoordinateBehindCamera) {
                        i++;
                        continue;
                    }

                    // The current index points to an on-screen node
                    // We want to start off-screen so that the sequence doesn't terminate on-screen
                    // Of course, if the index is 0, we can't decrement
                    if (i != 0) {
                        i--;
                    }

                    // Okay, we just found a coordinate that's on camera, let's start creating a sequence
                    vector<OrbitPoint> sequence;

                    // Add start of sequence - the first node might be on screen, so it'll look like the sequence
                    // starts on-screen even if it may start off-screen in actuality
                    //sequence.push_back(pair.second.at(i-1));

                    // Keep adding points to the sequence until the point is off-camera
                    while (i < pair.second.size()-1) {
                        sequence.push_back(pair.second.at(i));
                        i++;
                        
                        // Note that here we do checks. One check takes a vec3 and makes sure that the coordinate isn't actually BEHIND the camera
                        // The second check takes a vec2 and makes sure it isn't off-screen
                        vec3 scaledCoordinate = Rays::Scale(pair.second.at(i).position);
                        bool isCoordinateBehindCamera = Rays::IsCoordinateOffCamera(scaledCoordinate);
                        bool isCoordinateOffScreen = Rays::IsCoordinateOffCamera(Rays::WorldToScreen(scaledCoordinate));
                        if (isCoordinateOffScreen || isCoordinateBehindCamera) {
                            break;
                        }
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

        auto InterpolateSequences(unordered_map<string, vector<vector<OrbitPoint>>> &sequences) -> void {
            for (auto &pair : sequences) {
                for (auto &sequence : pair.second) {
                    for (int i = 0; i < sequence.size()-1; i++) {

                        // Find how close this point and the previous point are in screen space
                        float distance = glm::distance(Rays::WorldToScreen(sequence.at(i).position / SCALE_FACTOR), Rays::WorldToScreen(sequence.at(i+1).position / SCALE_FACTOR));

                        // If the points are close enough, interpolate them to create a new point in-between
                        if (distance > DISTANCE_THRESHOLD) {
                            Log(INFO, glm::to_string(Rays::WorldToScreen(sequence.at(i+1).position / SCALE_FACTOR)));
                            OrbitPoint newPoint = Simulation::Integrate(Bodies::GetMassiveBodies(), pair.first, sequence.at(i).timeToNextPoint / 2.0, sequence.at(i));
                            sequence.insert(sequence.begin() + i + 1, newPoint);

                            // Both the new and old OrbitPoints now have half the time to the next point
                            sequence.at(i).timeToNextPoint /= 2;
                            sequence.at(i+1).timeToNextPoint /= 2;

                            // We keep i the same because we want to check if this newly created point is sufficiently close to the previous one
                            i--;
                        }
                    }
                }
            }
        }

        auto TransformSequencesToScreen(unordered_map<string, vector<vector<OrbitPoint>>> &worldSequenceMap) -> unordered_map<string, vector<vector<vec2>>> {
            unordered_map<string, vector<vector<vec2>>> screenSequenceMap;

            // For every body
            for (auto &pair : worldSequenceMap) {
                vector<vector<vec2>> newSequenceVector;

                // For every sequence associated with that body
                for (vector<OrbitPoint> &oldSequenceVector : pair.second) {
                    vector<vec2> newSequence;

                    // For every point on that sequence
                    for (OrbitPoint &oldPoint : oldSequenceVector) {
                        newSequence.push_back(Rays::WorldToScreen(Rays::Scale(oldPoint.position)));
                    }

                    newSequenceVector.push_back(newSequence);
                }

                screenSequenceMap.insert(std::make_pair(pair.first, newSequenceVector));
            }

            return screenSequenceMap;
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
        unordered_map<string, vector<OrbitPoint>> unscaledPositionMap = Bodies::GetPositions();
        unordered_map<string, vector<vector<OrbitPoint>>> worldSequenceMap = ConvertToSequences(unscaledPositionMap);
        InterpolateSequences(worldSequenceMap);
        unordered_map<string, vector<vector<vec2>>> screenSequenceMap = TransformSequencesToScreen(worldSequenceMap);

        // Now we have a list of sequences, let's draw them
        // Iterate through every body
        vector<VERTEX_DATA_TYPE> vertices;
        for (const auto &pair : screenSequenceMap) {
            
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

                    // Make sure the direction is in the positive Y quadrant
                    // If we don't do this, the order of v3 and v4 may be swapped relative to other nodes
                    // which may cause the triangles to render improperly
                    if (direction.y < 0) {
                        direction.x = -direction.x;
                        direction.y = -direction.y;
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