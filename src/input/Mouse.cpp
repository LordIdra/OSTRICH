#include "Mouse.h"

#include "../util/Log.h"
#include <GLFW/glfw3.h>



// important: all variables set inside callbacks in this namespace are updated when glfwPollEvents() is called
namespace Mouse {

    namespace {
        float DOUBLE_CLICK_DURATION = 0.5;

        glm::vec2 positionPrevious;
        glm::vec2 position;
        glm::vec2 positionDelta;
        glm::vec2 scrollDelta;

        bool leftHeld;
        bool rightHeld;

        bool leftActive;
        float timeOfLastLeftClick; // seconds

        auto DefaultCallback() -> void {}

        void (*leftPress)() = DefaultCallback;
        void (*leftRelease)() = DefaultCallback;
        void (*leftDouble)() = DefaultCallback;

        void (*rightPress)() = DefaultCallback;
        void (*rightRelease)() = DefaultCallback;

        auto ButtonCallback(GLFWwindow* window, int button, int action, int mods) -> void {
            // press
            if (action == GLFW_PRESS) {
                // left
                if (button == GLFW_MOUSE_BUTTON_LEFT) {
                    leftHeld = true;
                    leftPress();
                    if (leftActive) {
                        leftDouble();
                    } else {
                        leftActive = true;
                        timeOfLastLeftClick = glfwGetTime();
                    }
                }
                // right
                if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                    Mouse::Hide();
                    rightHeld = true;
                    rightPress();
                }
            }

            // release
            else if (action == GLFW_RELEASE) {
                // left
                if (button == GLFW_MOUSE_BUTTON_LEFT) {
                    leftHeld = false;
                    leftRelease();
                }
                // right
                if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                    Mouse::Show();
                    rightHeld = false;
                    rightRelease();
                }
            }
        }

        auto MoveCallback(GLFWwindow* glfwWindow, double xpos, double ypos) -> void {
            // calculate how far the cursor has moved from its last position
            positionPrevious = position;
            position.x = (float)xpos / (float)Window::GetWidth();
            position.y = (float)ypos / (float)Window::GetHeight();
            positionDelta = position - positionPrevious;
        }

        auto ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) -> void {
            // calculate how far the scroll wheel has moved from its last position
            scrollDelta = glm::vec2(xoffset, yoffset);
        }
    }

    auto Init() -> void {
        glfwSetMouseButtonCallback(Window::GetWindow(), ButtonCallback);
        glfwSetCursorPosCallback(Window::GetWindow(), MoveCallback);
        glfwSetScrollCallback(Window::GetWindow(), ScrollCallback);
    }

    auto Update() -> void {
        positionDelta = vec2(0.0f, 0.0f);
        scrollDelta = vec2(0.0f, 0.0f);
        if (leftActive) {
            if (glfwGetTime() - timeOfLastLeftClick > DOUBLE_CLICK_DURATION) {
                leftActive = false;
            }
        }
    }

    auto LeftButtonHeld() -> bool {
        return leftHeld;
    }

    auto RightButtonHeld() -> bool {
        return rightHeld;
    }

    auto SetCallbackLeftPress(void (*function)()) -> void {
        leftPress = function;
    }

    auto SetCallbackLeftRelease(void (*function)()) -> void {
        leftRelease = function;
    }

    auto SetCallbackLeftDouble(void (*function)()) -> void {
        leftDouble = function;
    }

    auto SetCallbackRightPress(void (*function)()) -> void {
        rightPress = function;
    }

    auto SetCallbackRightRelease(void (*function)()) -> void {
        rightRelease = function;
    }

    auto GetPosition() -> glm::vec2 {
        return position;
    }

    auto GetPositionDelta() -> glm::vec2 {
        return positionDelta;
    }

    auto GetScrollDelta() -> glm::vec2 {
        // first vale = scrolling to the side
        // second value = normal scrolling
        return scrollDelta;
    }

    auto Show() -> void{
        glfwSetInputMode(Window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    auto Hide() -> void{
        glfwSetInputMode(Window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    }
}