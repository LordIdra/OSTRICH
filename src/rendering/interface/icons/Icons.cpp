#include "Icons.h"

#include "MassiveIcon.h"
#include "input/mouse/Buttons.h"
#include "input/mouse/Position.h"
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

using std::unique_ptr;
using std::make_unique;



namespace Icons {

    namespace {
        const double RADIUS_THRESHOLD = 0.005;
        const float MERGE_THRESHOLD = 24;
        const float HOVER_THRESHOLD = 14;
        const float SELECT_THRESHOLD = 20;
        const unsigned int STRIDE = 5;

        const float ICON_MAIN_RADIUS = 10;
        const float ICON_OUTLINE_RADIUS = 14;
        const float ICON_HOVER_INNER_RADIUS = 19;
        const float ICON_HOVER_OUTER_RADIUS = 24;
        const float ICON_SELECTED_RADIUS = 24;

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

        auto MouseOnIcon(const MassiveIcon &icon, float threshold) -> bool {
            // Mouse coordinates are currently between 0 and 1, let's normalize that so they are 0 to width and 0 to height
            vec2 c = Mouse::GetUnNormalizedPosition();

            const vec2 k = icon.GetScreenCoordinates();

            bool conditionY = abs(c.y - k.y) <  threshold;
            bool conditionX = abs(c.x - k.x) < (threshold - abs(c.y -  k.y));

            return conditionX && conditionY;
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

        auto AddIconCentre(vector<float> &vertices, const MassiveIcon &icon) -> void {
            vec2 centre = icon.GetNormalizedScreenCoordinates();
            float ICON_MAIN_RADIUS_X = ICON_MAIN_RADIUS / Window::GetWidth();
            float ICON_MAIN_RADIUS_Y = ICON_MAIN_RADIUS / Window::GetHeight();

            AddQuad(vertices, 
                IconVertex(centre.x, centre.y - ICON_MAIN_RADIUS_Y, icon.GetColor()),
                IconVertex(centre.x - ICON_MAIN_RADIUS_X, centre.y, icon.GetColor()),
                IconVertex(centre.x, centre.y + ICON_MAIN_RADIUS_Y, icon.GetColor()),
                IconVertex(centre.x + ICON_MAIN_RADIUS_X, centre.y, icon.GetColor()));
        }

        auto AddIconBorder(vector<float> &vertices, const MassiveIcon &icon) -> void {
            vec2 centre = icon.GetNormalizedScreenCoordinates();
            float ICON_OUTLINE_RADIUS_X = ICON_OUTLINE_RADIUS / Window::GetWidth();
            float ICON_OUTLINE_RADIUS_Y = ICON_OUTLINE_RADIUS / Window::GetHeight();

            AddQuad(vertices, 
                IconVertex(centre.x, centre.y - ICON_OUTLINE_RADIUS_Y, ICON_BORDER_COLOR),
                IconVertex(centre.x - ICON_OUTLINE_RADIUS_X, centre.y, ICON_BORDER_COLOR),
                IconVertex(centre.x, centre.y + ICON_OUTLINE_RADIUS_Y, ICON_BORDER_COLOR),
                IconVertex(centre.x + ICON_OUTLINE_RADIUS_X, centre.y, ICON_BORDER_COLOR));
        }

        auto AddIconSelected(vector<float> &vertices, const MassiveIcon &icon) -> void {
            vec2 centre = icon.GetNormalizedScreenCoordinates();
            float ICON_SELECTED_RADIUS_X = ICON_SELECTED_RADIUS / Window::GetWidth();
            float ICON_SELECTED_RADIUS_Y = ICON_SELECTED_RADIUS / Window::GetHeight();

            AddQuad(vertices, 
                IconVertex(centre.x, centre.y - ICON_SELECTED_RADIUS_Y, ICON_SELECTED_COLOR),
                IconVertex(centre.x - ICON_SELECTED_RADIUS_X, centre.y, ICON_SELECTED_COLOR),
                IconVertex(centre.x, centre.y + ICON_SELECTED_RADIUS_Y, ICON_SELECTED_COLOR),
                IconVertex(centre.x + ICON_SELECTED_RADIUS_X, centre.y, ICON_SELECTED_COLOR));
        }

        auto AddIconHover(vector<float> &vertices, const MassiveIcon &icon) -> void {
            vec2 centre = icon.GetNormalizedScreenCoordinates();
            float ICON_HOVER_INNER_RADIUS_X = ICON_HOVER_INNER_RADIUS / Window::GetWidth();
            float ICON_HOVER_INNER_RADIUS_Y = ICON_HOVER_INNER_RADIUS / Window::GetHeight();
            float ICON_HOVER_OUTER_RADIUS_X = ICON_HOVER_OUTER_RADIUS / Window::GetWidth();
            float ICON_HOVER_OUTER_RADIUS_Y = ICON_HOVER_OUTER_RADIUS / Window::GetHeight();

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

        auto AddVertices(vector<float> &vertices, const MassiveIcon &icon) -> void {
            AddIconCentre(vertices, icon);
            AddIconBorder(vertices, icon);
            if (Bodies::GetSelectedBody() == icon.GetBody().GetId()) {
                AddIconSelected(vertices, icon);
            }
            if (MouseOnIcon(icon, HOVER_THRESHOLD)) {
                AddIconHover(vertices, icon);
            }
        }

        auto IconsIntersect(const MassiveIcon &icon1, const MassiveIcon &icon2) -> bool {
            const vec2 k = icon1.GetScreenCoordinates();
            const vec2 c = icon2.GetScreenCoordinates();

            bool conditionY = abs(c.y - k.y) <  (MERGE_THRESHOLD);
            bool conditionX = abs(c.x - k.x) < ((MERGE_THRESHOLD) - abs(c.y -  k.y));

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

        auto GetMergedIcons() -> vector<MassiveIcon> {
            // Get a list of icons for all bodies
            vector<MassiveIcon> massiveIcons;
            for (const auto pair : Bodies::GetMassiveBodies()) {
                if (ShouldMassiveBeDrawn(pair.second)) {
                    massiveIcons.push_back(pair.second);
                }
            }

            // Merge icons
            while (FirstLayerMerge(massiveIcons)) {}

            return massiveIcons;
        }

        auto SwitchBodyBasedOnIcon() -> void {
            for (const MassiveIcon icon : GetMergedIcons()) {
                if (MouseOnIcon(icon, SELECT_THRESHOLD)) {
                    Bodies::SetSelectedBody(icon.GetBody().GetId());
                }
            }
        }
    }

    auto Init() -> void {
        // Register body switch function
        Mouse::SetCallbackLeftDouble(SwitchBodyBasedOnIcon);

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
        vector<MassiveIcon> massiveIcons = GetMergedIcons();

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