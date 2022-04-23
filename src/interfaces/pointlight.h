#pragma once
#include <glm/glm.hpp>

class Pointlight
{
public:
    int ID;

    int enabled;

    glm::vec3 color;

    glm::vec3 position;
    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};