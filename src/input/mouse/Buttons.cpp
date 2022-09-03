#include "Buttons.h"

#include <window/Window.h>
#include <util/Log.h>

#include <GLFW/glfw3.h>



// important: all variables set inside callbacks in this namespace are updated when glfwPollEvents() is called
namespace Mouse {

    namespace {
        const float DOUBLE_CLICK_DURATION = 0.5;

        bool leftHeld;
        bool rightHeld;

        bool leftActive;
        double timeOfLastLeftClick; // seconds

        auto DefaultCallback() -> void {}

        void (*leftPress)() = DefaultCallback;
        void (*leftRelease)() = DefaultCallback;
        void (*leftDouble)() = DefaultCallback;

        void (*rightPress)() = DefaultCallback;
        void (*rightRelease)() = DefaultCallback;

        auto ButtonCallbackPressLeft() -> void {
            // Double click stuff
            if (leftActive) {
                leftDouble();
            } else {
                leftActive = true;
                timeOfLastLeftClick = glfwGetTime();
            }

            // General callback stuff
            leftHeld = true;
            leftPress();
        }

        auto ButtonCallbackPressRight() -> void {
            // Mouse should be hidden when right mouse button held
            Mouse::Hide();

            // General callback stuff
            rightHeld = true;
            rightPress();
        }

        auto ButtonCallbackReleaseLeft() -> void {
            // General callback stuff
            leftHeld = false;
            leftRelease();
        }

        auto ButtonCallbackReleaseRight() -> void {
            // Mouse should be shown again when right mouse button released
            Mouse::Show();

            // General callback stuff
            rightHeld = false;
            rightRelease();
        }

        auto ButtonCallbackPress(const int button) -> void {
            switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                ButtonCallbackPressLeft();
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                ButtonCallbackPressRight();
                break;
            }
        }

        auto ButtonCallbackRelease(const int button) -> void {
            switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                ButtonCallbackReleaseLeft();
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                ButtonCallbackReleaseRight();
                break;
            }
        }

        auto ButtonCallback(GLFWwindow* window, int button, int action, int mods) -> void { // NOLINT(misc-unused-parameters)
            switch (action) {
            case GLFW_PRESS:
                ButtonCallbackPress(button);
                break;
            case GLFW_RELEASE:
                ButtonCallbackRelease(button);
                break;
            }
        }
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

    auto LeftButtonHeld() -> bool {
        return leftHeld;
    }

    auto RightButtonHeld() -> bool {
        return rightHeld;
    }

    auto Show() -> void{
        glfwSetInputMode(Window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    auto Hide() -> void{
        glfwSetInputMode(Window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    }
}