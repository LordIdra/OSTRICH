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

    auto GetPosition() -> glm::vec2 {
        return position;
    }

    auto GetPositionDelta() -> glm::vec2 {
        return positionDelta;
    }
}