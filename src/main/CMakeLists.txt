project("OSTRICH")

set(FILES
    "main.cpp"

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
    "util/Types.h"

    "rendering/Shader.h"
    "rendering/Shader.cpp"

    "rendering/Program.h"
    "rendering/Program.cpp"

    "rendering/VAO.h"
    "rendering/VAO.cpp"

    "rendering/camera.h"
    "rendering/camera.cpp"
)

set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")
include("~/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${FILES})

target_link_libraries (${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries (${PROJECT_NAME} PRIVATE glfw)