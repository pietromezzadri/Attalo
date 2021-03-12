#include "renderer.h"

Renderer::Renderer()
{

}

void Renderer::loadTexture(Texture* texture)
{
	glGenTextures(1, &texture->texture);
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    texture->data = stbi_load(texture->filename, &texture->width, &texture->height, &texture->nr_channels, 0);
    if (texture->data)
    {
        if (texture->alpha) 
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);

        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(texture->data);
}

void Renderer::activateTexture2d(Texture* texture)
{
    glActiveTexture(texture->glTexture);
	glBindTexture(GL_TEXTURE_2D, texture->texture);
}

void Renderer::setAttibutePoniters()
{
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    /* glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); */
}

void Renderer::drawArrays() { 
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::update(GLFWwindow* window)
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Renderer::clearScreen()
{   
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Renderer::~Renderer()
{
	glfwTerminate();
	std::cout << "terminated" << std::endl;
}