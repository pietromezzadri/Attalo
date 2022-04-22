#pragma once
#include <glm/glm.hpp>

class Spotlight
{
public:
    int enabled;

    glm::vec3  position;
    glm::vec3  direction;
    float cutOff;
    float outerCutOff;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
}