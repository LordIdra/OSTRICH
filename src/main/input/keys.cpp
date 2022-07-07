#include "keys.h"

#include <unordered_map>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "../window/window.h"



namespace keys {
    namespace {
        std::unordered_map<unsigned int, void(*)()> functionCalledWhenKeyPressed_;  // called only once, when the key is pressed
        std::unordered_map<unsigned int, void(*)()> functionCalledWhenKeyReleased_; // called continuously, when the key is held down

        auto ElementExists(std::unordered_map<unsigned int, void(*)()> map, unsigned int key) -> bool{
            // check if 'map' contains 'key'
            return (map.find(key) != map.end());
        }
        
        auto KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
            // called whenever a key press event fires while the window is in focus
            // checks which type of key press event it is
            // checks the corresponding map to see if there is a function entry for the key that was pressed
            // if there is a function entry, said function will be called
            switch (action) {
            case GLFW_PRESS:
                if (ElementExists(functionCalledWhenKeyPressed_, key)) {
                    functionCalledWhenKeyPressed_.at(key)();
                }
                break;

            case GLFW_RELEASE:
                if (ElementExists(functionCalledWhenKeyReleased_, key)) {
                    functionCalledWhenKeyReleased_.at(key)();
                }
                break;
            }
        }
    }

    auto Init() -> void {
        glfwSetKeyCallback(window::GetWindow(), KeyEventCallback);
    }

    auto BindFunctionToKeyPress(const int key, void (*function)()) -> void {
        functionCalledWhenKeyPressed_[key] = function;
    }
    
    auto BindFunctionToKeyRelease(const int key, void (*function)()) -> void {
        functionCalledWhenKeyReleased_[key] = function;
    }

    auto KeyHeldDown(int key) -> bool {
        return (glfwGetKey(window::GetWindow(), key) == GLFW_PRESS);
    }
}