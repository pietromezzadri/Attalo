#include "engine.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
/* template <typename T>
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset, T *object); */

Engine::Engine() 
{
    screen_width = 1280;
    screen_height = 720;
    title = "Attalo";
    version = "v0.0.2";
    const char* window_title = (std::string(title) + " " + std::string(version)).c_str();

    // Initialize OpenGL and GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create the context and window
    window = createWindow(screen_width, screen_height, window_title);
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
    const char* glsl_version = "#version 130";

    // Initialize shader
    Shader lightShader("../src/shaders/light_shader.vert", "../src/shaders/light_shader.frag");
    Shader materialShader("../src/shaders/material_shader.vert", "../src/shaders/material_shader.frag");

    Screen screen = Screen();

    screen.width = screen_width;
    screen.height = screen_height;

    Input input;
    Renderer renderer = Renderer();
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    Object colorCube = Object();
    Object lightCube = Object();

    glm::vec3 cubePos(0.0f, 0.0f, 0.0f);


    float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

	glGenVertexArrays(1, &colorCube.VAO);
    glGenVertexArrays(1, &lightCube.VAO);
    glGenBuffers(1, &colorCube.VBO);

    glBindBuffer(GL_ARRAY_BUFFER, colorCube.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(colorCube.VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(lightCube.VAO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    Texture texture0, texture1, boxTexture, boxSpecular;

    texture0.ID = 0;
    texture0.filename = "../images/container.jpg";
    texture0.glTexture = GL_TEXTURE0;

    texture1.ID = 1;
    texture1.filename = "../images/awesomeface.png";
    texture1.glTexture = GL_TEXTURE1;
    texture1.alpha = true;

    boxTexture.ID = 2;
    boxTexture.filename = "../images/container2.png";
    boxTexture.glTexture = GL_TEXTURE2;
    boxTexture.alpha = true;

    boxSpecular.ID = 3;
    boxSpecular.filename = "../images/container2_specular.png";
    boxSpecular.glTexture = GL_TEXTURE3;
    boxSpecular.alpha = true;

    renderer.loadTexture(&texture0);
    renderer.loadTexture(&texture1);
    renderer.loadTexture(&boxTexture);
    renderer.loadTexture(&boxSpecular);

    screen.deltaTime = 0;
    float lastFrame = 0;
    float currentFrame = 0;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    Spotlight spotlight = Spotlight();
    Pointlight pointlight = Pointlight();
    Pointlight pointlight2 = Pointlight();


    pointlight.position = glm::vec3(1.2f, 1.0f, 2.0f);
    pointlight2.position = glm::vec3(-2.0f, 0.0f, -3.0f);

    pointlight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    pointlight2.color = glm::vec3(0.0f, 1.0f, 0.0f);
    spotlight.color = glm::vec3(0.0f, 0.0f, 1.0f);

    pointlight.ID = 0;
    pointlight2.ID = 1;

    spotlight.ID = 0;

    AppConsole console = AppConsole();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        screen.deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (camera.mouseEnabled == false)
        {
            ImGui::Begin("Another Window", &camera.mouseEnabled);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            ImGui::ColorEdit3("Pointlight0", (float*)&pointlight.color);
            ImGui::ColorEdit3("Pointlight1", (float*)&pointlight2.color);
            ImGui::ColorEdit3("Spotlight0", (float*)&spotlight.color);

            if (ImGui::Button("Close Me"))
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                camera.mouseEnabled = true;
            }
            ImGui::End();
            console.Draw("Console");

            DocOpener().ShowExampleAppDocuments();
        }

        input.process_mouse(window, &camera);
        input.process_keyboard(window, &camera, screen.deltaTime);

        if (input.lastKey == GLFW_KEY_ESCAPE)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            camera.mouseEnabled = false;
            input.lastKey = 0;
        }

        renderer.clearScreen();

        renderer.activateTexture2d(&texture0);
        renderer.activateTexture2d(&texture1);
        renderer.activateTexture2d(&boxTexture);
        renderer.activateTexture2d(&boxSpecular);

        materialShader.use();
        materialShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        materialShader.setVec3("lightColor",  pointlight.color);
        materialShader.setVec3("viewPos", camera.Position);

        materialShader.setInt("material.diffuse", boxTexture.ID);
        materialShader.setInt("material.specular", boxSpecular.ID);
        materialShader.setFloat("material.shininess", 32.0f);

        pointlight.diffuse = pointlight.color * glm::vec3(0.5f);
        pointlight.ambient = pointlight.diffuse * glm::vec3(0.2f);

        pointlight2.diffuse = pointlight2.color * glm::vec3(0.5f);
        pointlight2.ambient = pointlight2.diffuse * glm::vec3(0.2f);

        spotlight.diffuse = spotlight.color * glm::vec3(0.5f);
        spotlight.ambient = spotlight.diffuse * glm::vec3(0.2f);

        // Point Light

        materialShader.setVec3("pointLight[0].ambient",  pointlight.ambient);
        materialShader.setVec3("pointLight[0].diffuse",  pointlight.diffuse); // darken diffuse pointLight a bit
        materialShader.setVec3("pointLight[0].specular", 1.0f, 1.0f, 1.0f);
        materialShader.setVec3("pointLight[0].position", pointlight.position);
        
        materialShader.setFloat("pointLight[0].constant", 1.0f);
        materialShader.setFloat("pointLight[0].linear", 0.09f);
        materialShader.setFloat("pointLight[0].quadratic", 0.032f);

        materialShader.setVec3("pointLight[1].ambient",  pointlight2.ambient);
        materialShader.setVec3("pointLight[1].diffuse",  pointlight2.diffuse); // darken diffuse pointLight a bit
        materialShader.setVec3("pointLight[1].specular", 1.0f, 1.0f, 1.0f);
        materialShader.setVec3("pointLight[1].position", pointlight2.position);
        
        materialShader.setFloat("pointLight[1].constant", 1.0f);
        materialShader.setFloat("pointLight[1].linear", 0.09f);
        materialShader.setFloat("pointLight[1].quadratic", 0.032f);

        // Spotlight

        materialShader.setVec3("spotLight[0].position",  camera.Position);
        materialShader.setVec3("spotLight[0].direction", camera.Front);
        materialShader.setFloat("spotLight[0].cutOff",   glm::cos(glm::radians(12.5f)));
        materialShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(17.5f)));

        materialShader.setVec3("spotLight[0].ambient",  spotlight.ambient);
        materialShader.setVec3("spotLight[0].diffuse",  spotlight.diffuse); // darken diffuse spotLight a bit
        materialShader.setVec3("spotLight[0].specular", 1.0f, 1.0f, 1.0f);
        
        materialShader.setFloat("spotLight[0].constant", 1.0f);
        materialShader.setFloat("spotLight[0].linear", 0.09f);
        materialShader.setFloat("spotLight[0].quadratic", 0.032f);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), screen.width / screen.height, 0.1f, 100.0f);

        

        // pass transformation matrices to the shader
        materialShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        
        glm::mat4 view = camera.GetViewMatrix();
        materialShader.setMat4("view", view);

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, cubePos);
        //model = glm::rotate(model, -glm::radians(35.f)*(float)glfwGetTime(), glm::vec3(0.0f,1.0f,1.0f));

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

        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            materialShader.setMat4("model", model);
            
            // render boxes
            glBindVertexArray(colorCube.VAO);

            renderer.drawArrays(GL_FILL);
        }

        lightShader.use();

        // pass transformation matrices to the shader
        lightShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        lightShader.setMat4("view", view);

        // calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointlight.position);
        model = glm::scale(model, glm::vec3(0.2f)); 
        
        lightShader.setMat4("model", model);
        lightShader.setVec4("lightColor", glm::vec4(pointlight.color, 1.0f));

        // render boxes
        glBindVertexArray(lightCube.VAO);

        renderer.drawArrays(GL_FILL);

        model = glm::mat4(1.0f);
        model = glm::translate(model, pointlight2.position);
        model = glm::scale(model, glm::vec3(0.2f)); 

        lightShader.setMat4("model", model);
        lightShader.setVec4("lightColor", glm::vec4(pointlight2.color, 1.0f));

        // render boxes
        glBindVertexArray(lightCube.VAO);

        renderer.drawArrays(GL_FILL);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        renderer.update(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &lightCube.VAO);
    glDeleteVertexArrays(1, &colorCube.VAO);
    glDeleteBuffers(1, &colorCube.VBO);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
/* template <typename T>
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset, T *object)
{
    object->ProcessMouseScroll(static_cast<float>(yoffset));
} */

Engine::~Engine() 
{
    glfwTerminate();
}

