#include "Position.h"

#include <window/Window.h>



namespace Mouse {

    namespace {
        vec2 positionPrevious;
        vec2 position;
        vec2 positionDelta;

        auto MoveCallback(GLFWwindow* glfwWindow, double xpos, double ypos) -> void { // NOLINT(misc-unused-parameters)
            // Calculate how far the cursor has moved from its last position
            positionPrevious = position;
            position.x = (float)xpos / (float)Window::GetWidth();
            position.y = (float)ypos / (float)Window::GetHeight();
            positionDelta = position - positionPrevious;
        }
    }

    auto GetNormalizedPosition() -> vec2 {
        return position;
    }

    auto GetScreenPosition() -> vec2 {
        // Invert y coordinates, so they start from the bottom left rather than top left
        vec2 position1 = vec2(position.x, 1 - position.y);

        // Linearise to the range -1 to 1
        vec2 position2 = vec2(position1.x*2 - 1, position1.y*2 - 1);

        return position2;
    }

    auto GetUnNormalizedPosition() -> vec2 {
        vec2 position = GetNormalizedPosition();

        // Invert Y
        position = vec2(position.x, 1 - position.y);

        // Normalise between width and height
        position *= vec2(Window::GetWidth(), Window::GetHeight());
        
        return position;
    }

    auto GetPositionDelta() -> glm::vec2 {
        return positionDelta;
    }
}