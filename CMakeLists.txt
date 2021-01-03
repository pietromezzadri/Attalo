cmake_minimum_required(VERSION 3.15)
project(Attalo VERSION 0.0.1)

include_directories("C:/MinGW/include")

add_subdirectory(glfw)

find_package(OpenGL REQUIRED)

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )


set(GLAD "C:/MinGW/include/glad/glad.h"
        "lib/src/glad.c")

set(SOURCE_FILES "src/main.cpp" "src/displayManager.cpp"
           "src/input.cpp" "src/renderer.cpp" "src/stb_image.cpp")


add_executable(Attalo ${SOURCE_FILES} ${GLAD})

target_link_libraries(Attalo OpenGL::GL)
target_link_libraries(Attalo glfw)
#target_link_libraries(Attalo dl)
