#pragma once
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "interfaces/screen.h"
#include "interfaces/texture.h"
#include "interfaces/object.h"
#include "interfaces/pointlight.h"
#include "interfaces/spotlight.h"
#include "renderer/shader.h"
#include "renderer/renderer.h"
#include "camera/camera.h"
#include "input/input.h"
#include "widgets/console.h"
#include "widgets/document.h"

#include <iostream>
#include <filesystem>
#include <string>

class Engine 
{
public:
    Engine();
    ~Engine();

public:
    void Run();
    void Stop();
    GLFWwindow* createWindow(int width, int height, const char* title);

public:
    unsigned int screen_width;
    unsigned int screen_height;
    const char* title;
    const char* version;
    bool running;
    GLFWwindow* window;
};