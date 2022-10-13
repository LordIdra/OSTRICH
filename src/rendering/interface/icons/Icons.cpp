#include "Icons.h"

#include <rendering/interface/icons/MassiveIcon.h>
#include <input/Mouse.h>
#include <rendering/interface/icons/IconVertex.h>
#include <window/Window.h>
#include <glm/gtx/string_cast.hpp>
#include <main/Bodies.h>
#include <rendering/VAO.h>
#include <rendering/shaders/Program.h>
#include <rendering/geometry/Rays.h>
#include <rendering/Texture.h>
#include <rendering/camera/Camera.h>
#include <rendering/camera/Settings.h>
#include <util/Log.h>
#include <rendering/interface/icons/Icon.h>

#include <glad/glad.h>

#include <string>

using std::unique_ptr;
using std::make_unique;



namespace Icons {

    namespace {
        const unsigned int STRIDE = 5;

        const float OFF_SCREEN_RADIUS = 1.1;

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
            if (screenCoords.x < -OFF_SCREEN_RADIUS || screenCoords.x > OFF_SCREEN_RADIUS || screenCoords.y < -OFF_SCREEN_RADIUS || screenCoords.y > OFF_SCREEN_RADIUS) {
                return false;
            }

            // If the body is off-camera, skip it           
            if (glm::degrees(angle) > FIELD_OF_VIEW) {
                return false;
            }

            // If the body is too large a radius, skip it
            if (sphereRadiusOnScreen > Icon::RADIUS_THRESHOLD) {
                return false;
            }

            return true;
        }

        auto IconsIntersect(const Icon &icon1, const Icon &icon2) -> bool {
            // Get icon screen coordinates
            const vec2 k = icon1.GetScreenCoordinates();
            const vec2 c = icon2.GetScreenCoordinates();

            // Check if both the X and Y conditions for the rhombus intersection are true
            bool conditionY = abs(c.y - k.y) <  (Icon::MERGE_THRESHOLD);
            bool conditionX = abs(c.x - k.x) < ((Icon::MERGE_THRESHOLD) - abs(c.y -  k.y));
            return conditionX && conditionY;
        }

        auto MergeIconIntoIcon(vector<MassiveIcon> &massiveIcons, int from, int to) -> void {
            massiveIcons[to].AddChild(massiveIcons[from].GetBody());
            massiveIcons.erase(std::next(massiveIcons.begin(), from));
        }

        auto MergeIcon(vector<MassiveIcon> &massiveIcons, int i, int j) -> void {
            // If i is the selected icon, merge j into i
            if (Bodies::GetSelectedBody() == massiveIcons[i].GetBody().GetId()) {
                MergeIconIntoIcon(massiveIcons, j, i);
                return;
            }

            // If i is the selected icon, merge i into j
            if (Bodies::GetSelectedBody() == massiveIcons[j].GetBody().GetId()) {
                MergeIconIntoIcon(massiveIcons, i, j);
                return;
            }

            // If the mass of icon i is greater, merge j into i
            if (massiveIcons[i].GetBody().GetMass() >= massiveIcons[j].GetBody().GetMass()) {
                MergeIconIntoIcon(massiveIcons, j, i);
                return;
            }

            // Otherwise, merge i into j
            MergeIconIntoIcon(massiveIcons, i, j);
        }

        auto DoOneIconMerge(vector<MassiveIcon> &massiveIcons) -> bool {
            for (int i = 0; i < massiveIcons.size(); i++) {
                for (int j = 0; j < massiveIcons.size(); j++) {

                    // Make sure we're not trying to merge the icon with itself...
                    if (i == j) {
                        continue;
                    }

                    // Check if the icons intersect - if so, merge them
                    if (IconsIntersect(massiveIcons[i], massiveIcons[j])) {
                        MergeIcon(massiveIcons, i, j);
                        return true;
                    }
                }
            }
            return false;
        }

        auto CheckOcclusion(vector<MassiveIcon> &massiveIcons) -> bool {
            for (int i = 0; i < massiveIcons.size(); i++) {
                for (const auto &pair : Bodies::GetMassiveBodies()) {

                    // Make sure we're not checking if the icon is occluded by its own parent (that would make no sense)
                    if (pair.first == massiveIcons[i].GetBody().GetId()) {
                        continue;
                    }

                    // If the body is further from the camera than the icon, then it cannot be occluding the icon
                    float distanceFromCameraToIcon = glm::length(Camera::GetPosition() - massiveIcons[i].GetBody().GetScaledPosition());
                    float distanceFromCameraToBody = glm::length(Camera::GetPosition() - pair.second.GetScaledPosition());
                    if (distanceFromCameraToBody > distanceFromCameraToIcon) {
                        continue;
                    }

                    // Check if the ray from the camera position in the direction of the icon intersects the body
                    vec3 direction = Rays::ScreenToWorld(massiveIcons[i].GetNormalizedScreenCoordinates());
                    if (Rays::IntersectsSphere(Camera::GetPosition(), direction, pair.second.GetScaledPosition(), pair.second.GetScaledRadius())) {
                        massiveIcons.erase(std::next(massiveIcons.begin(), i));
                        return true;
                    }
                }
            }
            return false;
        }

        auto GetMassiveIcons() -> vector<MassiveIcon> {
            // Get a list of icons for all bodies
            vector<MassiveIcon> massiveIcons;
            for (const auto &pair : Bodies::GetMassiveBodies()) {
                if (ShouldMassiveBeDrawn(pair.second)) {
                    massiveIcons.emplace_back(pair.second);
                }
            }

            // Merge icons
            while (DoOneIconMerge(massiveIcons)) {}
            while (CheckOcclusion(massiveIcons)) {}

            return massiveIcons;
        }

        auto SwitchBodyBasedOnIcon() -> void {
            for (const MassiveIcon &icon : GetMassiveIcons()) {
                if (icon.MouseOnIcon(Icon::SELECT_THRESHOLD)) {
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
        vector<MassiveIcon> massiveIcons = GetMassiveIcons();

        // Create a vector of vertices
        vector<float> data;
        for (const MassiveIcon &icon : massiveIcons) {
            icon.AddVertices(data);
        }

        // Showtime
        unsigned int vertexCount = data.size() / STRIDE;
        vao->Data(data, vertexCount, GL_STATIC_DRAW);
        program->Use();
        vao->Render();
    }
}