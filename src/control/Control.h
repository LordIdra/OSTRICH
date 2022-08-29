#pragma once

#include "../input/Keys.h"
#include "../input/Mouse.h"
#include "../rendering/Camera.h"
#include "../util/Log.h"
#include "../window/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>



namespace Control {
    auto Init(const string &windowTitle) -> void;
    auto Update() -> void;
}

