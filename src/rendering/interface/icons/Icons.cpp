#include "Icons.h"

#include "MassiveIcon.h"
#include "window/Window.h"
#include <main/Bodies.h>
#include <rendering/VAO.h>
#include <rendering/shaders/Program.h>
#include <rendering/geometry/Rays.h>
#include <rendering/Texture.h>
#include <rendering/camera/Camera.h>
#include <rendering/camera/Settings.h>

#include <glad/glad.h>

#include <string>
#include <util/Log.h>

using std::unique_ptr;
using std::make_unique;



namespace Icons {

    namespace {
        const double RADIUS_THRESHOLD = 0.01;
        const unsigned int STRIDE = 7;
        const float ICON_RADIUS = 30;
        const float ICON_OPACITY = 0.1;

        unique_ptr<VAO> vao;
        unique_ptr<Program> program;
        unique_ptr<Texture> massiveTexture;

        auto ShouldMassiveBeDrawn(const Massive &massive) -> bool {
            // World and screen coordinates of the body
            vec3 worldCoords = massive.GetScaledPosition();
            vec2 screenCoords = Rays::WorldToScreen(worldCoords);

            // Calculatee the angle between the camera direction and the vector from the camera to the body
            vec3 cameraDirection = glm::normalize(Camera::GetPosition() - Camera::GetTarget());
            vec3 bodyDirection = glm::normalize(Camera::GetPosition() - massive.GetScaledPosition());
            double angle = acos(glm::dot(cameraDirection, bodyDirection));

            // Find the radius of the circle drawn to represent the sphere on-screen
            double sphereRadiusOnScreen = Rays::RadiusOnScreen(massive);

            // If the body is off-screen, skip it
            if (screenCoords.x < -1.1 || screenCoords.x > 1.1 || screenCoords.y < -1.1 || screenCoords.y > 1.1) {
                return false;
            }

            // If the body is off-camera, skip it           
            if (glm::degrees(angle) > FIELD_OF_VIEW) {
                return false;
            }

            // If the body is too large a radius, skip it
            if (sphereRadiusOnScreen > RADIUS_THRESHOLD) {
                return false;
            }

            return true;
        }

        auto AddVertex(vector<float> &vertices, float vx, float vy, const vec3 colour, const float tx,const float ty) -> void {
            // Vertex position
            vertices.push_back(vx);
            vertices.push_back(vy);

            // Colour
            vertices.push_back(colour.r);
            vertices.push_back(colour.g);
            vertices.push_back(colour.b);

            // Texture coordinate
            vertices.push_back(tx);
            vertices.push_back(ty);
        }

        auto AddVertices(vector<float> &vertices, const MassiveIcon icon) -> void {
            vec2 centre = icon.GetScreenCoordinates();

            float ICON_RADIUS_X = ICON_RADIUS / Window::GetWidth();
            float ICON_RADIUS_Y = ICON_RADIUS / Window::GetHeight();

            AddVertex(vertices, centre.x - ICON_RADIUS_X, centre.y - ICON_RADIUS_Y, icon.GetColor(), 0, 0);
            AddVertex(vertices, centre.x - ICON_RADIUS_X, centre.y + ICON_RADIUS_Y, icon.GetColor(), 0, 1);
            AddVertex(vertices, centre.x + ICON_RADIUS_X, centre.y + ICON_RADIUS_Y, icon.GetColor(), 1, 1);

            AddVertex(vertices, centre.x - ICON_RADIUS_X, centre.y - ICON_RADIUS_Y, icon.GetColor(), 0, 0);
            AddVertex(vertices, centre.x + ICON_RADIUS_X, centre.y + ICON_RADIUS_Y, icon.GetColor(), 1, 1);
            AddVertex(vertices, centre.x + ICON_RADIUS_X, centre.y - ICON_RADIUS_Y, icon.GetColor(), 1, 0);
        }
    }



    auto Init() -> void {
        // Create texture
        massiveTexture = make_unique<Texture>("../resources/icons/Massive.png");

        // Create VAO
        vao = make_unique<VAO>();
        vao->Init();

        // Add vertex attributes
        vao->AddVertexAttribute(VertexAttribute{
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
        vao->AddVertexAttribute(VertexAttribute{
            .index = 2,
            .size = 2,
            .type = GL_FLOAT,
            .normalised = GL_FALSE,
            .stride = STRIDE * sizeof(float),
            .offset = (void*)(5 * sizeof(float))}); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
        
        // Compile shaders
        Shader vertex = Shader("../resources/shaders/icon-vertex.vsh", GL_VERTEX_SHADER);
        Shader fragment = Shader("../resources/shaders/icon-fragment.fsh", GL_FRAGMENT_SHADER);
        program = make_unique<Program>(vertex, fragment);
    }

    auto DrawIcons() -> void {

        // Compile a list of all massive bodies to have icons rendered
        vector<MassiveIcon> massiveIcons;
        for (const auto pair : Bodies::GetMassiveBodies()) {
            if (ShouldMassiveBeDrawn(pair.second)) {
                massiveIcons.push_back(pair.second);
            }
        }

        /* merge icons */

        // Create a vector of vertices
        vector<float> data;
        for (const MassiveIcon &icon : massiveIcons) {
            AddVertices(data, icon);
        }

        // Showtime
        unsigned int vertexCount = data.size() / STRIDE;
        vao->Data(data, vertexCount, GL_STATIC_DRAW);
        program->Use();
        massiveTexture->Bind();
        vao->Render();
    }
}