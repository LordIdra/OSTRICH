cmake_minimum_required(VERSION 3.16)
project("OSTRICH")

set(FILES
    "main.cpp"

    
    "bodies/Body.h"
    "bodies/Body.cpp"
    
    "bodies/Massive.h"
    "bodies/Massive.cpp"


    "control/Control.h"
    "control/Control.cpp"


    "input/Keys.h"
    "input/Keys.cpp"

    "input/Mouse.h"
    "input/Mouse.cpp"


    "window/Window.h"
    "window/Window.cpp"


    "util/Constants.h"

    "util/Log.h"
    "util/Log.cpp"

    "util/Types.h"


    "rendering/geometry/Rays.h"
    "rendering/geometry/Rays.cpp"

    "rendering/geometry/Transition.h"
    "rendering/geometry/Transition.cpp"

    "rendering/shaders/Shader.h"
    "rendering/shaders/Shader.cpp"

    "rendering/shaders/Program.h"
    "rendering/shaders/Program.cpp"

    "rendering/structures/Material.h"
    "rendering/structures/VertexAttribute.h"

    "rendering/Camera.h"
    "rendering/Camera.cpp"

    "rendering/Geometry.h"
    "rendering/Geometry.cpp"

    "rendering/VAO.h"
    "rendering/VAO.cpp"
)

find_program(CCACHE ccache)

set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=bugprone-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-misc-unused-using-decls,-cppcoreguidelines-pro-type-union-access,-readability-implicit-bool-conversion)

include("~/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME} ${FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ".")

target_link_libraries (${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries (${PROJECT_NAME} PRIVATE glfw)
target_link_libraries (${PROJECT_NAME} PRIVATE Boost::boost)