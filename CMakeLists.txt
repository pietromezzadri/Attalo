cmake_minimum_required(VERSION 3.15)
project(Attalo VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(glfw)
add_subdirectory(glm)

find_package(OpenGL REQUIRED)

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )


set(GLAD "lib/src/glad.c")
set(STB "lib/src/stb_image.cpp")

set(SOURCE_FILES "src/main.cpp" "src/camera.cpp"
           "src/input.cpp" "src/renderer.cpp"
           "src/engine.cpp")

add_executable(Attalo ${SOURCE_FILES} ${GLAD} ${STB})

target_link_libraries(Attalo OpenGL::GL)
target_link_libraries(Attalo glfw)
target_link_libraries(Attalo glm)
