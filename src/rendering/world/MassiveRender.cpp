#include "MassiveRender.h"
#include "util/Log.h"

#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <main/Bodies.h>
#include <rendering/camera/Camera.h>
#include <rendering/shaders/Program.h>
#include <rendering/VAO.h>
#include <util/Types.h>
#include <input/Mouse.h>

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using std::unique_ptr;
using std::make_unique;



namespace MassiveRender {
    namespace {
        const vec3 LIGHT_POSITION = vec3(0.0, 0.05, 0.0);
        const unsigned int STRIDE = 6;

        unordered_map<string, VAO> massive_vaos;
        unique_ptr<Program> program;

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
        // Program
        Shader vertex = Shader("../resources/shaders/massive-vertex.vsh", GL_VERTEX_SHADER);
        Shader fragment = Shader("../resources/shaders/massive-fragment.fsh", GL_FRAGMENT_SHADER);
        program = make_unique<Program>(vertex, fragment);
    }
    
    auto Reset() -> void {
        massive_vaos.clear();
    }

    auto Update(const double deltaTime) -> void {
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
}