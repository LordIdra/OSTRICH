cmake_minimum_required(VERSION 3.16)
project("OSTRICH")

# Internally built dependencies
set(DEPEND_FILES
    "src/depend/implot/implot_demo.cpp"
    "src/depend/implot/implot_internal.h"
    "src/depend/implot/implot_items.cpp"
    "src/depend/implot/implot.cpp"
    "src/depend/implot/implot.h"
)

# Source files
set(FILES
    "src/depend/stb_image.h"
    
    "src/main/Bodies.cpp"
    "src/main/Control.cpp"
    "src/main/Main.cpp"
    
    "src/scenarios/Scenarios.cpp"
    "src/scenarios/ScenarioFileUtil.cpp"
    "src/scenarios/YMLUtil.cpp"

    "src/bodies/Body.cpp"
    "src/bodies/Massive.cpp"

    "src/input/Mouse.cpp"
    "src/input/Keys.cpp"

    "src/window/Window.cpp"

    "src/util/Log.cpp"
    "src/util/TimeFormat.cpp"

    "src/rendering/camera/Camera.cpp"
    "src/rendering/camera/CameraTransition.cpp"
    "src/rendering/camera/Util.cpp"

    "src/rendering/geometry/Rays.cpp"
    "src/rendering/geometry/Transition.cpp"
    "src/rendering/geometry/Sphere.cpp"

    "src/rendering/world/MassiveRender.cpp"
    "src/rendering/world/OrbitPaths.cpp"
    "src/rendering/world/Icon.cpp"
    "src/rendering/world/Icons.cpp"

    "src/rendering/interface/BottomRightWindow/BottomRightWindow.cpp"
    "src/rendering/interface/BottomRightWindow/SimulationControl.cpp"
    "src/rendering/interface/BottomRightWindow/ScenarioTable.cpp"
    "src/rendering/interface/BottomRightWindow/SaveScenario.cpp"
    "src/rendering/interface/BottomRightWindow/LoadScenario.cpp"
    "src/rendering/interface/TopRightWindow/TopRightWindow.cpp"
    "src/rendering/interface/TopRightWindow/SimulationData.cpp"

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

    "src/simulation/Simulation.cpp"
    "src/simulation/SimulationEnergy.cpp"
    "src/simulation/SimulationState.cpp"
)

# Use vscode toolchain file
set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Export compile commands (better IDE integration)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Run vcpkg cmake file
include("~/vcpkg/scripts/buildsystems/vcpkg.cmake")

# C++ standard
set(CMAKE_CXX_STANDARD 17)

# Enable ccache
find_program(CCACHE_FOUND ccache)
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)

# Find external dependencies
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

# Build dependencies (without clang-tidy checks)
add_library(dependencies OBJECT ${DEPEND_FILES})
target_include_directories(dependencies PRIVATE "~/vcpkg/installed/x64-linux/include")
set_target_properties(dependencies PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries (dependencies PRIVATE imgui::imgui)

# Build Tracy
option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)
add_subdirectory(src/depend/tracy-0.9)

# Enable clang tidy checks
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -header-filter="" -checks=bugprone-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-misc-unused-using-decls,-cppcoreguidelines-pro-type-union-access,-readability-implicit-bool-conversion,-readability-magic-numbers,-bugprone-narrowing-conversions,-modernize-pass-by-value,-cppcoreguidelines-pro-type-vararg,-cppcoreguidelines-pro-bounds-array-to-pointer-decay)

# Build OSTRICH
add_executable(${PROJECT_NAME} ${FILES})

# Include src directory (easier to reference from #include directives)
target_include_directories(${PROJECT_NAME} PRIVATE "src")

# Link external libraries and internally built dependencies
target_link_libraries (${PROJECT_NAME} PRIVATE glad::glad glfw imgui::imgui yaml-cpp)
target_link_libraries (${PROJECT_NAME} PRIVATE dependencies Tracy::TracyClient)