#version 330 core
out vec4 FragColor;

uniform vec4 lightColor;

void main()
{
    FragColor = lightColor; // set alle 4 vector values to 1.0
}