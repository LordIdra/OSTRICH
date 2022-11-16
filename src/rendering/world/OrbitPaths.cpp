#include "OrbitPaths.h"
#include <simulation/OrbitPoint.h>
#include "simulation/Simulation.h"
#include "rendering/VAO.h"
#include "simulation/SimulationState.h"
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
#include <utility>

using std::unique_ptr;



namespace OrbitPaths {

    namespace {

        const int STRIDE = 6;
        const float PATH_WIDTH = 0.005;
        const vec3 PATH_COLOR = vec3(0.0, 0.0, 0.9);
        const float DISTANCE_THRESHOLD = 0.01;
        const float RATIO_OF_FUTURE_POINTS_TO_FADE_OVER = 0.05;
        const float PAST_POINT_BRIGHTNESS = 0.4;

        unique_ptr<VAO> futurePointsVAO;
        unique_ptr<VAO> pastPointsVAO;
        unique_ptr<Program> program;

        auto ScaleStateMap(vector<SimulationState> &unscaledPointMap) -> void {
            ZoneScoped;
            for (SimulationState &state : unscaledPointMap) {
                state.Scale();
            }
        }

        auto GetPositionMap(const vector<SimulationState> &futureStates) -> unordered_map<string, vector<vec3>> {
            ZoneScoped;
            unordered_map<string, vector<vec3>> positionMap;
            
            // Make sure every body has an entry in the map using the first state
            for (const auto &pair : futureStates.at(0).GetOrbitPoints()) {
                ZoneScoped;
                positionMap.insert(std::make_pair(pair.first, vector<vec3>()));
            }

            // Now add the positions for each body
            for (const SimulationState &state : futureStates) {
                ZoneScoped;
                for (const auto &pair : state.GetOrbitPoints()) {
                    positionMap.at(pair.first).push_back(pair.second.position);
                }
            }

            return positionMap;
        }

        auto AddVertex(vector<VERTEX_DATA_TYPE> &vertices, const vec3 position, const vec3 color) -> void {
            vertices.push_back(position.x);
            vertices.push_back(position.y);
            vertices.push_back(position.z);
            vertices.push_back(color.x);
            vertices.push_back(color.y);
            vertices.push_back(color.z);
        }

        auto DrawFuturePointMap(const unordered_map<string, vector<vec3>> &pointMap, const unsigned int drawMethod) -> void {
            ZoneScoped;
            // Iterate through every body
            for (const auto &pair : pointMap) {
                vector<VERTEX_DATA_TYPE> vertices;

                // Use color of the body for color of orbit path
                vec3 color = Bodies::GetBody(pair.first).GetColor();

                int numberOfPointsToFadeOver = int(pair.second.size() * RATIO_OF_FUTURE_POINTS_TO_FADE_OVER);

                // Iterate through every sequence for the corresponding body
                int i = 0;
                for (const vec3 &position : pair.second) {

                    // Figure out the color of this vertex
                    vec3 vertexColor = color;
                    if (pair.second.size() - i < numberOfPointsToFadeOver) {
                        float brightness = (float(pair.second.size() - i) / float(numberOfPointsToFadeOver));
                        vertexColor *= brightness;
                    }

                    i++;

                    // Add new vertices (two triangles to form a quadrilateral)
                    AddVertex(vertices, position, vertexColor);
                }

                program->Use();
                program->Set("cameraMatrix", Camera::GetMatrix());
                futurePointsVAO->Data(vertices, vertices.size() / STRIDE, GL_DYNAMIC_DRAW);
                futurePointsVAO->Render(drawMethod);
            }
        }

        auto DrawPastPointMap(unordered_map<string, vector<vec3>>  &positionMap, const unsigned int drawMethod) -> void {
            ZoneScoped;
            // Iterate through every body
            for (const auto &pair : positionMap) {
                vector<VERTEX_DATA_TYPE> vertices;

                // Use color of the body for color of orbit path
                vec3 color = Bodies::GetBody(pair.first).GetColor();

                int numberOfPointsToFadeOver = pair.second.size();

                // Iterate through every sequence for the corresponding body
                int i = 0;
                for (const vec3 &position : pair.second) {

                    // Figure out the color of this vertex
                    float brightness = float(i) / float(numberOfPointsToFadeOver);
                    vec3 vertexColor = color * brightness * PAST_POINT_BRIGHTNESS;
                    i++;

                    // Add new vertices (two triangles to form a quadrilateral)
                    AddVertex(vertices, position, vertexColor);
                }

                program->Use();
                program->Set("cameraMatrix", Camera::GetMatrix());
                futurePointsVAO->Data(vertices, vertices.size() / STRIDE, GL_DYNAMIC_DRAW);
                futurePointsVAO->Render(drawMethod);
            }
        }
    }

    auto Init() -> void {
        // Create Program
        Shader vertex = Shader("../resources/shaders/path-vertex.vsh", GL_VERTEX_SHADER);
        Shader fragment = Shader("../resources/shaders/path-fragment.fsh", GL_FRAGMENT_SHADER);
        program = std::make_unique<Program>(vertex, fragment);

        // Create future points VAO
        futurePointsVAO = std::make_unique<VAO>();
        futurePointsVAO->Init();
        futurePointsVAO->AddVertexAttribute(
            VertexAttribute{
            .index = 0,
            .size = 3,
            .type = GL_FLOAT,
            .normalised = GL_FALSE,
            .stride = STRIDE * sizeof(float),
            .offset = nullptr});
        futurePointsVAO->AddVertexAttribute(VertexAttribute{
            .index = 1,
            .size = 3,
            .type = GL_FLOAT,
            .normalised = GL_FALSE,
            .stride = STRIDE * sizeof(float),
            .offset = (void*)(3 * sizeof(float))}); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)

        // Create past points VAO
        pastPointsVAO = std::make_unique<VAO>();
        pastPointsVAO->Init();
        pastPointsVAO->AddVertexAttribute(
            VertexAttribute{
            .index = 0,
            .size = 3,
            .type = GL_FLOAT,
            .normalised = GL_FALSE,
            .stride = STRIDE * sizeof(float),
            .offset = nullptr});
        pastPointsVAO->AddVertexAttribute(VertexAttribute{
            .index = 1,
            .size = 3,
            .type = GL_FLOAT,
            .normalised = GL_FALSE,
            .stride = STRIDE * sizeof(float),
            .offset = (void*)(3 * sizeof(float))}); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
    }

    auto Update() -> void {
        ZoneScoped;
        // Future points
        vector<SimulationState> futureStates = Simulation::GetFutureStates();
        if (!futureStates.empty()) {
            ScaleStateMap(futureStates);
            unordered_map<string, vector<vec3>> futurePoints = GetPositionMap(futureStates);
            DrawFuturePointMap(futurePoints, GL_POINTS);
        }

        // Past points
        vector<SimulationState> pastStates = Simulation::GetPastStates();
        if (!pastStates.empty()) {
            ScaleStateMap(pastStates);
            unordered_map<string, vector<vec3>> pastPoints = GetPositionMap(pastStates);
            DrawPastPointMap(pastPoints, GL_LINE_STRIP);
        }
    }
}