#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>
#include <vector>
#include <iostream>

#include "texture.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

public:
	void update(GLFWwindow* window);
	void clearScreen();
	void loadTexture(Texture* texture);
	void activateTexture2d(Texture* textures);
	void setAttibutePoniters();
	void drawArrays();

public:
	Texture texture;
	glm::mat4 model;

};

