#include "engine.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Engine::Engine() 
{
    screen_width = 800;
    screen_height = 600;
    title = "Attalo";
    version = "v0.0.1";

    // Initialize OpenGL and GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create the context and window
    window = createWindow(screen_width, screen_height, title);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        Stop();
    }

    // Enable GL configs
    glEnable(GL_DEPTH_TEST);
}

GLFWwindow* Engine::createWindow(int width, int height, const char* title)
{
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) 
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        Stop();
    }
    return window;
}

void Engine::Run() 
{
    // Initialize shader
    Shader lightShader("../src/light_shader.vert", "../src/light_shader.frag");
    Shader colorShader("../src/color_shader.vert", "../src/color_shader.frag");

    Screen screen = Screen();

    screen.width = screen_width;
    screen.height = screen_height;

    Input input = Input();
    Renderer renderer = Renderer();
    Camera camera = Camera(&screen);
    Object colorCube = Object();
    Object lightCube = Object();

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 lightPos2(-1.2f, 1.0f, -3.0f);


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

	glGenVertexArrays(1, &colorCube.VAO);
    glGenVertexArrays(1, &lightCube.VAO);
    glGenBuffers(1, &colorCube.VBO);

    glBindBuffer(GL_ARRAY_BUFFER, colorCube.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(colorCube.VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(lightCube.VAO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    /* Texture texture1, texture2;

    texture1.filename = "../src/container.jpg";
    texture1.glTexture = GL_TEXTURE0;

    texture2.filename = "../src/awesomeface.png";
    texture2.glTexture = GL_TEXTURE1;
    texture2.alpha = true;

    renderer.loadTexture(&texture1);
    renderer.loadTexture(&texture2); */


    screen.deltaTime = 0;
    float lastFrame = 0;
    float currentFrame = 0;
    

    camera.eye  = glm::vec3(0.0f, 0.0f,  3.0f);
    camera.center = glm::vec3(0.0f, 0.0f, -1.0f);
    camera.up    = glm::vec3(0.0f, 1.0f,  0.0f);
    
    input.mouse.x = screen.width/2;
    input.mouse.y = screen.height/2;
    input.mouse.lastX = input.mouse.x;
    input.mouse.lastY = input.mouse.y;
    input.mouse.sensitivity = 0.08f;
    input.mouse.yaw = -90.0f;
    input.mouse.pitch = 0.0f;
    input.mouse.roll = 0.0f;


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        screen.deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.speed = 20.0f * screen.deltaTime;
        
        input.processMouse(window, &camera);
        input.processInput(window, &camera);

        camera.eye.y = 0.0f;

        renderer.clearScreen();

        /* renderer.activateTexture2d(&texture1);
        renderer.activateTexture2d(&texture2); */

        colorShader.use();
        colorShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        colorShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        colorShader.setVec3("lightPos", lightPos);

        camera.createTransformations(&screen);

        // pass transformation matrices to the shader
        colorShader.setMat4("projection", camera.projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        colorShader.setMat4("view", camera.view);

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        
        colorShader.setMat4("model", model);

        // render boxes
        glBindVertexArray(colorCube.VAO);

        renderer.drawArrays();

        lightShader.use();

        // pass transformation matrices to the shader
        lightShader.setMat4("projection", camera.projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        lightShader.setMat4("view", camera.view);

        // calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); 
        
        lightShader.setMat4("model", model);

        // render boxes
        glBindVertexArray(lightCube.VAO);

        renderer.drawArrays();

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos2);
        model = glm::scale(model, glm::vec3(0.2f)); 
        
        lightShader.setMat4("model", model);

        // render boxes
        glBindVertexArray(lightCube.VAO);

        renderer.drawArrays();


        renderer.update(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &lightCube.VAO);
    glDeleteVertexArrays(1, &colorCube.VAO);
    glDeleteBuffers(1, &colorCube.VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

void Engine::Stop() 
{
    running = false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Engine::~Engine() 
{
    glfwTerminate();
}

