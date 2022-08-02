#include "keys.h"

#include <unordered_map>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "../window/window.h"



namespace keys {
    namespace {
        std::unordered_map<unsigned int, void(*)()> functionCalledWhenKeyPressed;  // called only once, when the key is pressed
        std::unordered_map<unsigned int, void(*)()> functionCalledWhenKeyReleased; // called only once, when the key is released
        std::unordered_map<unsigned int, void(*)()> functionCalledWhenKeyHeld;     // called continuously, when the key is held
        std::unordered_map<unsigned int, bool> isKeyHeld;                          // stores if the key is being held down

        auto ElementExists(std::unordered_map<unsigned int, void(*)()> map, unsigned int key) -> bool{
            // check if 'map' contains 'key'
            return (map.find(key) != map.end());
        }
        
        auto KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
            // called whenever a key press event fires while the window is in focus
            // checks which type of key press event it is
            // checks the corresponding map to see if there is a function entry for the key that was pressed
            // if there is a function entry, said function will be called
            // also, the isKeyHeld value will (always) be set according to the type of press
            switch (action) {
            case GLFW_PRESS:
                isKeyHeld[key] = true;
                if (ElementExists(functionCalledWhenKeyPressed, key)) {
                    functionCalledWhenKeyPressed.at(key)();
                }
                break;

            case GLFW_RELEASE:
                isKeyHeld[key] = false;
                if (ElementExists(functionCalledWhenKeyReleased, key)) {
                    functionCalledWhenKeyReleased.at(key)();
                }
                break;
            }
        }
    }

    auto Init() -> void {
        glfwSetKeyCallback(window::GetWindow(), KeyEventCallback);
    }

    auto Update() -> void {
        for (const auto &pair :functionCalledWhenKeyHeld) {
            if (isKeyHeld.at(pair.first)) {
                pair.second();
            }
        }
    }

    auto BindFunctionToKeyPress(const int key, void (*function)()) -> void {
        functionCalledWhenKeyPressed[key] = function;
    }
    
    auto BindFunctionToKeyRelease(const int key, void (*function)()) -> void {
        functionCalledWhenKeyReleased[key] = function;
    }

    auto BindFunctionToKeyHold(const int key, void (*function)()) -> void {
        functionCalledWhenKeyHeld[key] = function;
        isKeyHeld[key] = false;
    }

    auto KeyHeldDown(int key) -> bool {
        return (glfwGetKey(window::GetWindow(), key) == GLFW_PRESS);
    }
}