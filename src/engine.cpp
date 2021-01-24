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
    Shader mainShader("../src/shader.vert", "../src/shader.frag");

    Screen screen = Screen();

    screen.width = screen_width;
    screen.height = screen_height;

    Input input = Input();
    Renderer renderer = Renderer();
    Camera camera = Camera(&screen);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    renderer.loadBuffers(vertices);

    glBindBuffer(GL_ARRAY_BUFFER, renderer.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    renderer.setAttibutePoniters();
    
    Texture texture1, texture2;

    texture1.filename = "../src/container.jpg";
    texture1.glTexture = GL_TEXTURE0;

    texture2.filename = "../src/awesomeface.png";
    texture2.glTexture = GL_TEXTURE1;
    texture2.alpha = true;

    renderer.loadTexture(&texture1);
    renderer.loadTexture(&texture2);

    mainShader.use();
    mainShader.setInt("texture1", 0);
    mainShader.setInt("texture2", 1);

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

        renderer.activateTexture2d(&texture1);
        renderer.activateTexture2d(&texture2);

        mainShader.use();

        camera.createTransformations(&screen);

        // pass transformation matrices to the shader
        mainShader.setMat4("projection", camera.projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        mainShader.setMat4("view", camera.view);
        mainShader.setFloat("alpha", 0.2f);

        // render boxes
        glBindVertexArray(renderer.VAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (1.0f + i);
            model = (i % 2 == 0 ? glm::rotate(model, glm::radians(angle) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f)) : 
                glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)) );
            
            mainShader.setMat4("model", model);

            renderer.drawArrays();
        }

        renderer.update(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &renderer.VAO);
    glDeleteBuffers(1, &renderer.VBO);

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

