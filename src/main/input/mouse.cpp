#include "mouse.h"

#include "../util/logging.h"



// important: the delta variables in this namespace are updated when glfwPollEvents() is called
namespace mouse {
    namespace {
        glm::vec2 positionPrevious;
        glm::vec2 position;
        glm::vec2 positionDelta;
        glm::vec2 scrollDelta;

        auto MoveCallback(GLFWwindow* glfwWindow, double xpos, double ypos) -> void {
            // calculate how far the cursor has moved from its last position
            positionPrevious = position;
            position.x = (float)xpos / (float)window::GetWidth();
            position.y = (float)ypos / (float)window::GetHeight();
            positionDelta = position - positionPrevious;
        }

        auto ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) -> void {
            // calculate how far the scroll wheel has moved from its last position
            scrollDelta = glm::vec2(xoffset, yoffset);
        }
    }

    auto Init() -> void {
        glfwSetCursorPosCallback(window::GetWindow(), MoveCallback);
        glfwSetScrollCallback(window::GetWindow(), ScrollCallback);
    }

    auto Update() -> void {
        positionDelta = vec2(0.0f, 0.0f);
        scrollDelta = vec2(0.0f, 0.0f);
    }

    auto GetPositionDelta() -> glm::vec2 {
        return positionDelta;
    }

    auto GetScrollDelta() -> glm::vec2 {
        // first vale = scrolling to the side
        // second value = normal scrolling
        return scrollDelta;
    }

    void Show() {
        glfwSetInputMode(window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Hide() {
        glfwSetInputMode(window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    }
}