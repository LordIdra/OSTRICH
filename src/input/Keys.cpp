#include "Keys.h"

#include <window/Window.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>



namespace Keys {
    namespace {
        unordered_map<unsigned int, void(*)()> functionCalledWhenKeyPressed;  // Called only once, when the key is pressed
        unordered_map<unsigned int, void(*)()> functionCalledWhenKeyReleased; // Called only once, when the key is released
        unordered_map<unsigned int, void(*)()> functionCalledWhenKeyHeld;     // Called continuously, when the key is held
        unordered_map<unsigned int, bool> isKeyHeld;                          // Stores if the key is being held down

        auto ElementExists(std::unordered_map<unsigned int, void(*)()> map, unsigned int key) -> bool{
            // Check if 'map' contains 'key'
            return (map.find(key) != map.end());
        }
        
        auto KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void { // NOLINT(misc-unused-parameters)
            // Called whenever a key press event fires while the window is in focus
            // Checks which type of key press event it is
            // Checks the corresponding map to see if there is a function entry for the key that was pressed
            // If there is a function entry, said function will be called
            // Also, the isKeyHeld value will (always) be set according to the type of press
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
        glfwSetKeyCallback(Window::GetWindow(), KeyEventCallback);
    }

    auto Update() -> void {
        // Loop through every entry in the functionCalledWhenKeyHeld map
        for (const auto &pair : functionCalledWhenKeyHeld) {

            // Check if the key is being held down
            if (isKeyHeld.at(pair.first)) {
                
                // Call the function (value)
                pair.second();
            }
        }
    }

    auto KeyHeldDown(int key) -> bool {
        return (glfwGetKey(Window::GetWindow(), key) == GLFW_PRESS);
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
}