cmake_minimum_required(VERSION 3.16)
project("OSTRICH")

set(FILES
    "src/depend/stb_image.h"
    
    "src/main/Bodies.cpp"
    "src/main/Control.cpp"
    "src/main/Main.cpp"
    "src/main/Scenarios.cpp"
    "src/main/Simulation.cpp"

    "src/bodies/Body.cpp"
    "src/bodies/Massive.cpp"

    "src/input/Mouse.cpp"
    "src/input/Keys.cpp"

    "src/window/Window.cpp"

    "src/util/Log.cpp"

    "src/rendering/camera/Camera.cpp"
    "src/rendering/camera/Util.cpp"

    "src/rendering/geometry/Rays.cpp"
    "src/rendering/geometry/Transition.cpp"
    "src/rendering/geometry/Sphere.cpp"

    "src/rendering/world/MassiveRender.cpp"
    "src/rendering/world/OrbitPaths.cpp"
    "src/rendering/world/Icon.cpp"
    "src/rendering/world/Icons.cpp"

    "src/rendering/interface/BottomRightWindow/BottomRightWindow.cpp"

    "src/rendering/interface/LeftWindow/LeftWindow.cpp"
    "src/rendering/interface/LeftWindow/BodyData.cpp"
    "src/rendering/interface/LeftWindow/Explorer.cpp"

    "src/rendering/interface/Fonts.cpp"
    "src/rendering/interface/Interface.cpp"
    "src/rendering/interface/Style.cpp"

    "src/rendering/shaders/Program.cpp"
    "src/rendering/shaders/Shader.cpp"
    "src/rendering/shaders/Util.cpp"

    "src/rendering/Texture.cpp"
    "src/rendering/VAO.cpp"
)

# Enable clang tidy checks
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=bugprone-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-misc-unused-using-decls,-cppcoreguidelines-pro-type-union-access,-readability-implicit-bool-conversion,-readability-magic-numbers,-bugprone-narrowing-conversions,-modernize-pass-by-value,-cppcoreguidelines-pro-type-vararg)

set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})

include("~/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${FILES} ${DEPEND_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE "src")
target_link_libraries (${PROJECT_NAME} PRIVATE glad::glad glfw imgui::imgui yaml-cpp)