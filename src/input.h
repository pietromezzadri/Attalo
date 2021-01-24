#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

#include "camera.h"
#include "mouse.h"

class Input
{
public:
	Input();
	~Input();

public:
	Mouse mouse;

public:
	void processInput(GLFWwindow* window, Camera* camera);
	void processMouse(GLFWwindow* window, Camera* camera);
	void processScroll(Camera* camera, double xoffset, double yoffset);

private:
	void performAction(const char* state, const char* input);

};

