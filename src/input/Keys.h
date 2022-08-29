#pragma once

#include "../window/Window.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <unordered_map>

using std::unordered_map;



namespace Keys {
    auto Init() -> void;
    auto Update() -> void;
    auto BindFunctionToKeyPress(const int key, void (*function)()) -> void;
    auto BindFunctionToKeyRelease(const int key, void (*function)()) -> void;
    auto BindFunctionToKeyHold(const int key, void (*function)()) -> void;
    auto KeyHeldDown(int key) -> bool;
}