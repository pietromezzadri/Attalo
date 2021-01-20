#include "input.h"

Input::Input()
{

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Input::processInput(GLFWwindow *window, Camera* camera)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->eye += camera->speed * camera->center;
		
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->eye -= camera->speed * camera->center;
		
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->eye -= camera->speed * glm::normalize(glm::cross(camera->center, camera->up));
		
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->eye += camera->speed * glm::normalize(glm::cross(camera->center, camera->up));
		
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
	}
}

void Input::processMouse(GLFWwindow* window, Camera* camera)
{
	glfwGetCursorPos(window, &mouse.x, &mouse.y);
	mouse.xOffset = mouse.x - mouse.lastX;
	mouse.yOffset = mouse.lastY - mouse.y;
	
	mouse.lastX = mouse.x;
	mouse.lastY = mouse.y;

	mouse.xOffset *= mouse.sensitivity;
	mouse.yOffset *= mouse.sensitivity;

	mouse.yaw += mouse.xOffset;
	mouse.pitch += mouse.yOffset;

	mouse.pitch = mouse.pitch > 89.0f ? 89.0f : mouse.pitch;
	mouse.pitch = mouse.pitch < -89.0f ? -89.0f : mouse.pitch;

	mouse.direction.x = cos(glm::radians(mouse.yaw)) * cos(glm::radians(mouse.pitch));
	mouse.direction.y = sin(glm::radians(mouse.pitch));
	mouse.direction.z = sin(glm::radians(mouse.yaw)) * cos(glm::radians(mouse.pitch));
	
	camera->center = glm::normalize(mouse.direction);
}

void Input::performAction(const char* state, const char* input)
{
	
}


Input::~Input()
{

}