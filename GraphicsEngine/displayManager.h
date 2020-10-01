#pragma once
#include <GLFW/glfw3.h>

class displayManager
{
public:
	displayManager();
	~displayManager();

public:
	GLFWwindow* getWindow(int win_width, int win_heigth, const char *title);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void update(GLFWwindow* window);
};

