cmake_minimum_required(VERSION 3.16)
project("OSTRICH")

set(FILES
    "main.cpp"

    "bodies/Body.h"
    "bodies/Body.cpp"
    
    "bodies/Massive.h"
    "bodies/Massive.cpp"

    "control/control.h"
    "control/control.cpp"

    "input/keys.h"
    "input/keys.cpp"

    "input/mouse.h"
    "input/mouse.cpp"

    "window/window.h"
    "window/window.cpp"

    "util/Constants.h"

    "util/logging.h"
    "util/logging.cpp"

    "util/rays.h"
    "util/rays.cpp"

    "util/transition.cpp"

    "util/Types.h"

    "rendering/camera.h"
    "rendering/camera.cpp"

    "rendering/geometry.cpp"
    "rendering/geometry.h"

    "rendering/Material.h"

    "rendering/Program.h"
    "rendering/Program.cpp"

    "rendering/Shader.h"
    "rendering/Shader.cpp"

    "rendering/VAO.h"
    "rendering/VAO.cpp"

    "rendering/VertexAttribute.h"

)

set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")
include("~/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME} ${FILES})

target_link_libraries (${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries (${PROJECT_NAME} PRIVATE glfw)
target_link_libraries (${PROJECT_NAME} PRIVATE Boost::boost)