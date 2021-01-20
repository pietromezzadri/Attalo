#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "screen.h"


class Camera
{
public:
    Camera(Screen* screen);
    ~Camera();

public:
    void createTransformations();

public:
    glm::vec3 eye, center, up, position;
    glm::mat4 view, projection;
    unsigned int ID;
    float FOV, x, y, z, speed;
};