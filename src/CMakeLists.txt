cmake_minimum_required(VERSION 3.16)
project("OSTRICH")

set(FILES
    "main/Bodies.cpp"
    "main/Control.cpp"
    "main/Main.cpp"
    "main/Render.cpp"
    "main/Simulation.cpp"

    "bodies/Body.cpp"
    "bodies/Massive.cpp"


    "input/Mouse.cpp"
    "input/Keys.cpp"

    "window/Window.cpp"

    "util/Log.cpp"

    "rendering/camera/Camera.cpp"
    "rendering/camera/Util.cpp"
    "rendering/geometry/Rays.cpp"
    "rendering/geometry/Transition.cpp"
    "rendering/geometry/Sphere.cpp"
    "rendering/shaders/Program.cpp"
    "rendering/shaders/Shader.cpp"
    "rendering/shaders/Util.cpp"
    "rendering/VAO.cpp"
)

#find_program(CCACHE ccache)

set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=bugprone-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-misc-unused-using-decls,-cppcoreguidelines-pro-type-union-access,-readability-implicit-bool-conversion,-readability-magic-numbers,-bugprone-narrowing-conversions)

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