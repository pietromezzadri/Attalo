#pragma once
#include <GLFW/glfw3.h>

class input
{
public:
	input();
	~input();

public:
	void processInput(GLFWwindow *window);

};

