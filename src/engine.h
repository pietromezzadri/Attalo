#pragma once
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <stb_image/stb_image.h>

#include "screen.h"
#include "texture.h"
#include "shader.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "object.h"

#include <iostream>
#include <filesystem>

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