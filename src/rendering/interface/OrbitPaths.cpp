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
#include <rendering/camera/Camera.h>
#include <string>
#include <unordered_map>
#include <util/Types.h>
#include <main/Bodies.h>

#include <glad/glad.h>

using std::unique_ptr;



namespace OrbitPaths {

    namespace {

        const int STRIDE = 6;
        const float PATH_WIDTH = 0.005;
        const vec3 PATH_COLOR = vec3(0.0, 0.0, 0.9);
        const float DISTANCE_THRESHOLD = 0.01;
        const float RATIO_OF_POINTS_TO_FADE_OVER = 0.2;

        unique_ptr<VAO> vao;
        unique_ptr<Program> program;

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

        auto TransformSequencesToScreen(unordered_map<string, vector<OrbitPoint>> &unscaledPointMap) -> unordered_map<string, vector<vec3>> {
            unordered_map<string, vector<vec3>> worldPositionMap;

            // For every body
            for (auto &pair : unscaledPointMap) {
                vector<vec3> newPositionVector;

                // For every sequence associated with that body
                for (OrbitPoint &point : pair.second) {
                    newPositionVector.push_back(Rays::Scale(point.position));
                }

                worldPositionMap.insert(std::make_pair(pair.first, newPositionVector));
            }

            return worldPositionMap;
        }

        auto AddVertex(vector<VERTEX_DATA_TYPE> &vertices, const vec3 position, const vec3 color) -> void {
            vertices.push_back(position.x);
            vertices.push_back(position.y);
            vertices.push_back(position.z);
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
            .size = 3,
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
            .offset = (void*)(3 * sizeof(float))}); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
    }

    auto Update() -> void {
        unordered_map<string, vector<OrbitPoint>> unscaledPointMap = Bodies::GetPositions();
        unordered_map<string, vector<vec3>> worldPositionMap = TransformSequencesToScreen(unscaledPointMap);

        // Now we have a list of sequences, let's draw them
        // Iterate through every body
        for (const auto &pair : worldPositionMap) {
            vector<VERTEX_DATA_TYPE> vertices;

            // Use color of the body for color of orbit path
            vec3 color = Bodies::GetBody(pair.first).GetColor();

            int numberOfPointsToFadeOver = pair.second.size() * RATIO_OF_POINTS_TO_FADE_OVER;
            
            // Iterate through every sequence for the corresponding body
            int i = 0;
            for (const vec3 &position : pair.second) {

                // Figure out the color of this vertex
                vec3 vertexColor = color;
                if (pair.second.size() - i < numberOfPointsToFadeOver) {
                    float brightness = (float(pair.second.size() - i) / numberOfPointsToFadeOver);
                    vertexColor *= brightness;
                }

                i++;

                // Add new vertices (two triangles to form a quadrilateral)
                AddVertex(vertices, position, vertexColor);
            }

            program->Use();
            program->Set("cameraMatrix", Camera::GetMatrix());
            vao->Data(vertices, vertices.size() / STRIDE, GL_DYNAMIC_DRAW);
            vao->Render(GL_LINE_STRIP);
        }
    }
}