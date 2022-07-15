#include "control/control.h"

#include "input/keys.h"
#include "input/mouse.h"

#include "rendering/Program.h"
#include "rendering/VAO.h"
#include "rendering/camera.h"

#include "rendering/geometry.h"
#include "util/Types.h"
#include "util/logging.h"
#include "window/window.h"


#include <GLFW/glfw3.h>



const Material material {
    .ambient = vec3(0.02, 0.05, 0.07),
    .diffuse = vec3(0.1, 0.25, 0.35),
    .specular = vec3(0.2, 0.5, 0.7),
    .shine = 64.0
};

Program program;
VAO vao;

auto InitialiseProgram() -> void {
    Shader vertex;
    Shader fragment;
    vertex.Init("../../resource/vertex.vsh", GL_VERTEX_SHADER);
    fragment.Init("../../resource/fragment.fsh", GL_FRAGMENT_SHADER);
    program.Init();
    program.AddShader(vertex);
    program.AddShader(fragment);
    program.Link();
}

auto InitialiseVAO() -> void {
    vao.Init();
    vao.AddVertexAttribute(VertexAttribute{
        .index = 0,
        .size = 3,
        .type = GL_FLOAT,
        .normalised = GL_FALSE,
        .stride = 6 * sizeof(float),
        .offset = nullptr});
    vao.AddVertexAttribute(VertexAttribute{
        .index = 1,
        .size = 3,
        .type = GL_FLOAT,
        .normalised = GL_FALSE,
        .stride = 6 * sizeof(float),
        .offset = (void*)(3 * sizeof(float))});

    vector<VERTEX_DATA_TYPE> data1 = geometry::Sphere(vec3(0.0, 0.0, 0.0), 1.0, PI/96);
    vector<VERTEX_DATA_TYPE> data2 = geometry::Sphere(vec3(6.0, 0.0, 2.0), 0.5, PI/96);
    vector<VERTEX_DATA_TYPE> data3 = geometry::Sphere(vec3(4.0, 0.0, -3.0), 0.7, PI/96);

    data1.insert(data1.end(), data2.begin(), data2.end());
    data1.insert(data1.end(), data3.begin(), data3.end());

    unsigned int vertexCount = data1.size() / 6;
    vao.Data(data1, vertexCount,  GL_STATIC_DRAW);
}

auto Initialisecamera() -> void {
    camera::SetTarget(vec3(0.0f, 0.0f, 0.0f));
}

auto InitialiseInput() -> void {
    mouse::Hide();
}

auto WireFrame(bool enabled) -> void {
    if (enabled) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

auto main() -> int {
    control::Init("TEST!");

    InitialiseProgram();
    InitialiseVAO();
    Initialisecamera();
    InitialiseInput();

    //WireFrame(true);

    while (!window::ShouldClose()) {
        window::Background(vec4(0.0, 0.0, 0.0, 1.0));
        camera::AddAngleDelta(mouse::GetPositionDelta());
        camera::AddZoomDelta(mouse::GetScrollDelta().y);

        vec3 light_position(9.0, 2.0, 1.0);

        program.Use();
        program.Set("cameraMatrix", camera::GetMatrix());
        program.Set("cameraPosition", camera::GetPosition());
        program.Set("lightPosition", light_position);
        program.Set("material", material);

        vao.Render();
        control::Update();
    }

    // 0 = successful execution
    return 0;
}
