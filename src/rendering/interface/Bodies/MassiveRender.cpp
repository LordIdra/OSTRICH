#include "MassiveRender.h"
#include "util/Log.h"

#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <main/Bodies.h>
#include <rendering/geometry/Transition.h>
#include <rendering/camera/Camera.h>
#include <rendering/shaders/Program.h>
#include <rendering/VAO.h>
#include <string>
#include <util/Types.h>
#include <input/Mouse.h>
#include <input/Keys.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using std::unique_ptr;
using std::make_unique;



namespace MassiveRender {
    namespace {
        const float KEY_ZOOM_AMOUNT = 0.2;
        const vec3 ZERO_VECTOR = vec3(0, 0, 0);
        const vec3 LIGHT_POSITION = vec3(0.0, 0.05, 0.0);
        const unsigned int STRIDE = 6;
        const float TRANSITION_TIME = 0.2;

        unordered_map<string, VAO> massive_vaos;
        Transition transition = Transition(ZERO_VECTOR, ZERO_VECTOR, 0.0, 0.0, 0.0);
        unique_ptr<Program> program;

        auto KeyZoomIn() -> void {
            Camera::AddZoomDelta(KEY_ZOOM_AMOUNT);
        }

        auto KeyZoomOut() -> void {
            Camera::AddZoomDelta(-KEY_ZOOM_AMOUNT);
        }

        auto AddVertexAttributes(VAO &vao) -> void {
            vao.AddVertexAttribute(VertexAttribute{
                .index = 0,
                .size = 3,
                .type = GL_FLOAT,
                .normalised = GL_FALSE,
                .stride = STRIDE * sizeof(float),
                .offset = nullptr});
            vao.AddVertexAttribute(VertexAttribute{
                .index = 1,
                .size = 3,
                .type = GL_FLOAT,
                .normalised = GL_FALSE,
                .stride = STRIDE * sizeof(float),
                .offset = (void*)(3 * sizeof(float))}); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
        }
    }

    auto Init() -> void {
        // Input
        Keys::BindFunctionToKeyHold(GLFW_KEY_EQUAL, KeyZoomIn);
        Keys::BindFunctionToKeyHold(GLFW_KEY_MINUS, KeyZoomOut);
        
        // Program
        Shader vertex = Shader("../resources/shaders/massive-vertex.vsh", GL_VERTEX_SHADER);
        Shader fragment = Shader("../resources/shaders/massive-fragment.fsh", GL_FRAGMENT_SHADER);
        program = make_unique<Program>(vertex, fragment);
    }

    auto Update(double deltaTime) -> void {
        // Update the camera target according to the transition
        transition.Step(deltaTime);

        // Set program variables
        program->Use();
        program->Set("cameraMatrix", Camera::GetMatrix());
        program->Set("cameraPosition", Camera::GetPosition());
        program->Set("lightPosition", vec3(sin(glfwGetTime()), 0, cos(glfwGetTime())));

        // Render every VAO
        for (const auto &pair: massive_vaos) {
            const VAO &vao = pair.second;
            const Massive &body = Bodies::GetMassiveBody(pair.first);
            program->Set("modelMatrix", body.GetMatrix());
            program->Set("material", body.GetMaterial());
            vao.Render(GL_TRIANGLES);
        }
    }

    auto AddBody(const Massive &body) -> void {
        // Add a corresponding VAO for the body
        massive_vaos.insert(std::make_pair(body.GetId(), VAO()));
        VAO &vao = massive_vaos.at(body.GetId());
        vao.Init();

        // Add VAO vertex attributes
        AddVertexAttributes(vao);

        // Set VAO data
        vector<VERTEX_DATA_TYPE> data = body.GetSphereVertices();
        unsigned int vertexCount = data.size() / STRIDE;
        vao.Data(data, vertexCount,  GL_STATIC_DRAW);
    }

    auto StartTransition(const Body &body) -> void {
        transition = Transition(
            Camera::GetTarget(), 
            body.GetScaledPosition(), 
            Camera::GetZoom(), 
            Bodies::GetMinZoom(),
            TRANSITION_TIME);
    }

    auto UpdateTransitionTarget(const Body &body) -> void {
        transition.UpdateTarget(body.GetScaledPosition());
    }
}