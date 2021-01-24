#include "camera.h"

Camera::Camera(Screen* screen)
{
    speed = 0.2f;
    view = glm::mat4(1.0f);
    FOV = 45.0f;
    projection = glm::mat4(1.0f);
}

void Camera::createTransformations(Screen* screen)
{
    projection = glm::perspective(glm::radians(FOV), (float)screen->width / (float)screen->height, 0.1f, 100.0f);   
    view = glm::lookAt(eye, eye + center, up);
}

Camera::~Camera()
{
    
}