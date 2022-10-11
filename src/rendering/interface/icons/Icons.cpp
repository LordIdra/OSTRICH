#include "Icons.h"

#include "MassiveIcon.h"
#include "rendering/interface/icons/IconVertex.h"
#include "window/Window.h"
#include <glm/gtx/string_cast.hpp>
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
        const double RADIUS_THRESHOLD = 0.005;
        const unsigned int STRIDE = 5;

        const float ICON_MAIN_RADIUS = 15;
        const float ICON_OUTLINE_RADIUS = 19;
        const float ICON_HOVER_INNER_RADIUS = 25;
        const float ICON_HOVER_OUTER_RADIUS = 28;
        const float ICON_SELECTED_RADIUS = 25;

        const vec3 ICON_BORDER_COLOR = vec3(1.0, 0.7, 0.1);
        const vec3 ICON_HOVER_COLOR = vec3(1.0, 0.2, 0.1);
        const vec3 ICON_SELECTED_COLOR = vec3(0.8, 0.2, 0.1);

        unique_ptr<VAO> vao;
        unique_ptr<Program> program;

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

        auto AddVertex(vector<float> &vertices, IconVertex vertex) -> void {
            // Vertex position
            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);

            // Colour
            vertices.push_back(vertex.r);
            vertices.push_back(vertex.g);
            vertices.push_back(vertex.b);
        }

        auto AddQuad(vector<float> &vertices, IconVertex v1, IconVertex v2, IconVertex v3, IconVertex v4) -> void {
            AddVertex(vertices, v1);
            AddVertex(vertices, v2);
            AddVertex(vertices, v3);

            AddVertex(vertices, v1);
            AddVertex(vertices, v3);
            AddVertex(vertices, v4);
        }

        auto AddVertices(vector<float> &vertices, const MassiveIcon icon) -> void {
            vec2 centre = icon.GetNormalizedScreenCoordinates();

            float ICON_MAIN_RADIUS_X = ICON_MAIN_RADIUS / Window::GetWidth();
            float ICON_MAIN_RADIUS_Y = ICON_MAIN_RADIUS / Window::GetHeight();

            float ICON_OUTLINE_RADIUS_X = ICON_OUTLINE_RADIUS / Window::GetWidth();
            float ICON_OUTLINE_RADIUS_Y = ICON_OUTLINE_RADIUS / Window::GetHeight();

            float ICON_HOVER_INNER_RADIUS_X = ICON_HOVER_INNER_RADIUS / Window::GetWidth();
            float ICON_HOVER_INNER_RADIUS_Y = ICON_HOVER_INNER_RADIUS / Window::GetHeight();

            float ICON_HOVER_OUTER_RADIUS_X = ICON_HOVER_OUTER_RADIUS / Window::GetWidth();
            float ICON_HOVER_OUTER_RADIUS_Y = ICON_HOVER_OUTER_RADIUS / Window::GetHeight();

            float ICON_SELECTED_RADIUS_X = ICON_SELECTED_RADIUS / Window::GetWidth();
            float ICON_SELECTED_RADIUS_Y = ICON_SELECTED_RADIUS / Window::GetHeight();

            // Centre
            AddQuad(vertices, 
                IconVertex(centre.x, centre.y - ICON_MAIN_RADIUS_Y, icon.GetColor()),
                IconVertex(centre.x - ICON_MAIN_RADIUS_X, centre.y, icon.GetColor()),
                IconVertex(centre.x, centre.y + ICON_MAIN_RADIUS_Y, icon.GetColor()),
                IconVertex(centre.x + ICON_MAIN_RADIUS_X, centre.y, icon.GetColor()));

            // Borders
            AddQuad(vertices, 
                IconVertex(centre.x, centre.y - ICON_OUTLINE_RADIUS_Y, ICON_BORDER_COLOR),
                IconVertex(centre.x - ICON_OUTLINE_RADIUS_X, centre.y, ICON_BORDER_COLOR),
                IconVertex(centre.x, centre.y + ICON_OUTLINE_RADIUS_Y, ICON_BORDER_COLOR),
                IconVertex(centre.x + ICON_OUTLINE_RADIUS_X, centre.y, ICON_BORDER_COLOR));

            // Selected
            AddQuad(vertices, 
                IconVertex(centre.x, centre.y - ICON_SELECTED_RADIUS_Y, ICON_SELECTED_COLOR),
                IconVertex(centre.x - ICON_SELECTED_RADIUS_X, centre.y, ICON_SELECTED_COLOR),
                IconVertex(centre.x, centre.y + ICON_SELECTED_RADIUS_Y, ICON_SELECTED_COLOR),
                IconVertex(centre.x + ICON_SELECTED_RADIUS_X, centre.y, ICON_SELECTED_COLOR));
        
            // Hover
            AddQuad(vertices,
                IconVertex(centre.x, centre.y - ICON_HOVER_INNER_RADIUS_Y, ICON_HOVER_COLOR),
                IconVertex(centre.x - ICON_HOVER_INNER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
                IconVertex(centre.x - ICON_HOVER_OUTER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
                IconVertex(centre.x, centre.y - ICON_HOVER_OUTER_RADIUS_Y, ICON_HOVER_COLOR));
            
            AddQuad(vertices,
                IconVertex(centre.x, centre.y - ICON_HOVER_INNER_RADIUS_Y, ICON_HOVER_COLOR),
                IconVertex(centre.x + ICON_HOVER_INNER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
                IconVertex(centre.x + ICON_HOVER_OUTER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
                IconVertex(centre.x, centre.y - ICON_HOVER_OUTER_RADIUS_Y, ICON_HOVER_COLOR));

            AddQuad(vertices,
                IconVertex(centre.x, centre.y + ICON_HOVER_INNER_RADIUS_Y, ICON_HOVER_COLOR),
                IconVertex(centre.x - ICON_HOVER_INNER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
                IconVertex(centre.x - ICON_HOVER_OUTER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
                IconVertex(centre.x, centre.y + ICON_HOVER_OUTER_RADIUS_Y, ICON_HOVER_COLOR));

            AddQuad(vertices,
                IconVertex(centre.x, centre.y + ICON_HOVER_INNER_RADIUS_Y, ICON_HOVER_COLOR),
                IconVertex(centre.x + ICON_HOVER_INNER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
                IconVertex(centre.x + ICON_HOVER_OUTER_RADIUS_X, centre.y, ICON_HOVER_COLOR),
                IconVertex(centre.x, centre.y + ICON_HOVER_OUTER_RADIUS_Y, ICON_HOVER_COLOR));
        }

        auto IconsIntersect(const MassiveIcon &icon1, const MassiveIcon &icon2) -> bool {
            const vec2 k = icon1.GetNormalizedScreenCoordinates();
            const vec2 c = icon2.GetNormalizedScreenCoordinates();

            bool conditionY = abs(c.y - k.y) <  (2 * ICON_MAIN_RADIUS / Window::GetHeight());
            bool conditionX = abs(c.x - k.x) < ((2 * ICON_MAIN_RADIUS / Window::GetWidth()) - abs(c.y -  k.y));

            return conditionX && conditionY;
        }

        auto StartIconMerge(vector<MassiveIcon> &massiveIcons, int i, int j) -> void {
            if (massiveIcons[i].GetBody().GetMass() >= massiveIcons[j].GetBody().GetMass()) {
                massiveIcons[i].AddChild(massiveIcons[j].GetBody());
                massiveIcons.erase(std::next(massiveIcons.begin(), j));
            } else {
                massiveIcons[j].AddChild(massiveIcons[i].GetBody());
                massiveIcons.erase(std::next(massiveIcons.begin(), i));
            }
        }

        auto SecondLayerMerge(vector<MassiveIcon> &massiveIcons, int i) -> bool {
            for (int j = 0; j < massiveIcons.size(); j++) {
                if (i == j) {
                    continue;
                }

                if (IconsIntersect(massiveIcons[i], massiveIcons[j])) {
                    StartIconMerge(massiveIcons, i, j);
                    return true;
                }
            }
            return false;
        }

        auto FirstLayerMerge(vector<MassiveIcon> &massiveIcons) -> bool {
            for (int i = 0; i < massiveIcons.size(); i++) {
                if (SecondLayerMerge(massiveIcons, i)) {
                    return true;
                }
            }
            return false;
        }
    }



    auto Init() -> void {
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

        while (FirstLayerMerge(massiveIcons)) {}

        // Create a vector of vertices
        vector<float> data;
        for (const MassiveIcon &icon : massiveIcons) {
            AddVertices(data, icon);
        }

        // Showtime
        unsigned int vertexCount = data.size() / STRIDE;
        vao->Data(data, vertexCount, GL_STATIC_DRAW);
        program->Use();
        vao->Render();
    }
}