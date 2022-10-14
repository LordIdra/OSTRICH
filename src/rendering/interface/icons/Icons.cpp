#include "Icons.h"
#include "rendering/interface/icons/MasslessIcon.h"

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

        auto IsBodyOffScreen(const Body &body) -> bool {
            vec3 worldCoords = body.GetScaledPosition();
            vec2 screenCoords = Rays::WorldToScreen(worldCoords);
            return (screenCoords.x < -OFF_SCREEN_RADIUS || screenCoords.x > OFF_SCREEN_RADIUS || screenCoords.y < -OFF_SCREEN_RADIUS || screenCoords.y > OFF_SCREEN_RADIUS);
        }

        auto IsBodyOffCamera(const Body &body) -> bool {
            vec3 cameraDirection = glm::normalize(Camera::GetPosition() - Camera::GetTarget());
            vec3 bodyDirection = glm::normalize(Camera::GetPosition() - body.GetScaledPosition());
            double angle = acos(glm::dot(cameraDirection, bodyDirection));
            return glm::degrees(angle) > FIELD_OF_VIEW;
        }

        auto IsBodyOccluded(Body const &body) -> bool {
            // Occlusion check
            for (const auto &pair : Bodies::GetMassiveBodies()) {

                // Make sure we're not checking if the body occludes itself
                if (pair.second.GetId() == body.GetId()) {
                    continue;
                }

                // If the body is closer to the camera than the icon, then it may be occluding the icon
                float distanceFromCameraToIcon = glm::length(Camera::GetPosition() - body.GetScaledPosition());
                float distanceFromCameraToBody = glm::length(Camera::GetPosition() - pair.second.GetScaledPosition());
                if (distanceFromCameraToBody < distanceFromCameraToIcon) {

                    // Check if it is in fact occluding the icon
                    // The icon will be rendered in the middle of the body, so it makes sense to just consider the middle of the body (as a single point)
                    vec3 direction1 = Rays::ScreenToWorld(Rays::WorldToScreen(body.GetScaledPosition()));
                    if (Rays::IntersectsSphere(Camera::GetPosition(), direction1, pair.second.GetScaledPosition(), pair.second.GetScaledRadius())) {
                        return true;
                    }
                }
            }

            return false;
        }

        auto IsBodyTooClose(const Massive &massive) -> bool {
            return Rays::RadiusOnScreen(massive) > Icon::RADIUS_THRESHOLD;
        }

        auto ShouldMassiveBeDrawn(const Massive &massive) -> bool {
            return !(IsBodyOffScreen(massive) || IsBodyOffCamera(massive) || IsBodyOccluded(massive) || IsBodyTooClose(massive));
        }

        auto ShouldMasslessBeDrawn(const Massless &massless) -> bool {
            return !(IsBodyOffScreen(massless) || IsBodyOffCamera(massless) || IsBodyOccluded(massless));
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

        auto MergeIconIntoIcon(unordered_map<string, MassiveIcon> &massiveIcons, string from, string to) -> void {
            massiveIcons.at(to).AddChild(massiveIcons.at(from).GetBody());
            massiveIcons.erase(from);
        }

        auto MergeIcon(unordered_map<string, MassiveIcon> &massiveIcons, string icon1, string icon2) -> void {
            // If i is the selected icon, merge j into i
            if (Bodies::GetSelectedBody() == massiveIcons.at(icon1).GetBody().GetId()) {
                MergeIconIntoIcon(massiveIcons, icon2, icon1);
                return;
            }

            // If i is the selected icon, merge i into j
            if (Bodies::GetSelectedBody() == massiveIcons.at(icon2).GetBody().GetId()) {
                MergeIconIntoIcon(massiveIcons, icon1, icon2);
                return;
            }

            // If the mass of icon i is greater, merge j into i
            if (massiveIcons.at(icon1).GetBody().GetMass() >= massiveIcons.at(icon2).GetBody().GetMass()) {
                MergeIconIntoIcon(massiveIcons, icon2, icon1);
                return;
            }

            // Otherwise, merge i into j
            MergeIconIntoIcon(massiveIcons, icon1, icon2);
        }

        auto DoOneIconMerge(unordered_map<string, MassiveIcon> &massiveIcons) -> bool {
            for (auto const &pair1 : massiveIcons) {
                for (auto const &pair2 : massiveIcons) {

                    // Make sure we're not trying to merge the icon with itself...
                    if (pair1.first == pair2.first) {
                        continue;
                    }

                    // Check if the icons intersect - if so, merge them
                    if (IconsIntersect(pair1.second, pair2.second)) {
                        MergeIcon(massiveIcons, pair1.first, pair2.first);
                        return true;
                    }
                }
            }

            return false;
        }

        auto GetMassiveIcons() -> unordered_map<string, MassiveIcon> {
            unordered_map<string, MassiveIcon> massiveIcons;
            for (const auto &pair : Bodies::GetMassiveBodies()) {
                if (ShouldMassiveBeDrawn(pair.second)) {
                    massiveIcons.insert(std::pair<string, MassiveIcon>(pair.first, pair.second));
                }
            }

            return massiveIcons;
        }

        auto GetMasslessIcons() -> vector<MasslessIcon> {
            vector<MasslessIcon> masslessIcons;
            for (const auto &pair : Bodies::GetMasslessBodies()) {
                if (ShouldMasslessBeDrawn(pair.second)) {
                    masslessIcons.emplace_back(pair.second);
                }
            }

            return masslessIcons;
        }

        auto SwitchBodyBasedOnIcon() -> void {
            for (const auto &pair : GetMassiveIcons()) {
                if (pair.second.MouseOnIcon(Icon::SELECT_THRESHOLD)) {
                    Bodies::SetSelectedBody(pair.second.GetBody().GetId());
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
        unordered_map<string, MassiveIcon> massiveIcons = GetMassiveIcons();
        //vector<MasslessIcon> masslessIcons = GetMasslessIcons();

        // Merge icons
        while (DoOneIconMerge(massiveIcons)) {}

        // Create a vector of vertices
        vector<float> data;
        for (const auto &pair : massiveIcons) {
            pair.second.AddVertices(data);
        }

        // Showtime
        unsigned int vertexCount = data.size() / STRIDE;
        vao->Data(data, vertexCount, GL_STATIC_DRAW);
        program->Use();
        vao->Render();
    }
}