project("OSTRICH")

set(FILES
    "main.cpp"
    
    "window/window.h"
    "window/window.cpp"

    "util/logging.h"
    "util/logging.cpp"
    "util/Types.h"

    "control/control.h"
    "control/control.cpp"
)

set(CMAKE_TOOLCHAIN_FILE "/home/idra/vcpkg/scripts/buildsystems/vcpkg.cmake")
include("/home/idra/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${FILES})

target_link_libraries (${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries (${PROJECT_NAME} PRIVATE glfw)