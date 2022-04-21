#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "mouse.h"

class Input
{
public:
	Mouse mouse;

public:
	template <typename T>
	void process_keyboard(GLFWwindow *window, T *object, float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			object->ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			object->ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			object->ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			object->ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			object->ProcessKeyboard(UP, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			object->ProcessKeyboard(DOWN, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			object->ProcessKeyboard(MV_SPEED, 2);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
			object->ProcessKeyboard(MV_SPEED, 1);
	}


	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	template <typename T>
	void process_mouse(GLFWwindow* window, T *object)
	{
		glfwGetCursorPos(window, &mouse.x, &mouse.y);

		mouse.xOffset = mouse.x - mouse.lastX;
		mouse.yOffset = mouse.lastY - mouse.y;

		mouse.lastX = mouse.x;
		mouse.lastY = mouse.y;

		object->ProcessMouseMovement(mouse.xOffset, mouse.yOffset);
	}
	

private:
	void performAction(const char* state, const char* input);

};

