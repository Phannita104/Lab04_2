// Lab04.cpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"flat out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

// Fragment Shader
const char* fragmentShaderSource = "#version 330 core\n"
"flat in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH, SCR_HEIGHT, "Lab04", NULL, NULL
    );

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex Shader Error:\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment Shader Error:\n" << infoLog << std::endl;
    }

    // Link shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader Linking Error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 6 triangles, each with one solid color
    // Each vertex stores x, y, z, r, g, b

    float vertices[] = {
        // Triangle 1: RED
         0.0000f,  0.5000f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.3536f,  0.3536f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5000f,  0.0000f, 0.0f, 1.0f, 0.0f, 0.0f,

         // Triangle 2: ORANGE
          0.0000f,  0.5000f, 0.0f, 1.0f, 0.5f, 0.0f,
          0.5000f,  0.0000f, 0.0f, 1.0f, 0.5f, 0.0f,
          0.3536f, -0.3536f, 0.0f, 1.0f, 0.5f, 0.0f,

          // Triangle 3: YELLOW
           0.0000f,  0.5000f, 0.0f, 1.0f, 1.0f, 0.0f,
           0.3536f, -0.3536f, 0.0f, 1.0f, 1.0f, 0.0f,
           0.0000f, -0.5000f, 0.0f, 1.0f, 1.0f, 0.0f,

           // Triangle 4: GREEN
            0.0000f,  0.5000f, 0.0f, 0.0f, 0.8f, 0.0f,
            0.0000f, -0.5000f, 0.0f, 0.0f, 0.8f, 0.0f,
           -0.3536f, -0.3536f, 0.0f, 0.0f, 0.8f, 0.0f,

           // Triangle 5: BLUE
            0.0000f,  0.5000f, 0.0f, 0.0f, 0.3f, 1.0f,
           -0.3536f, -0.3536f, 0.0f, 0.0f, 0.3f, 1.0f,
           -0.5000f,  0.0000f, 0.0f, 0.0f, 0.3f, 1.0f,

           // Triangle 6: PURPLE
            0.0000f,  0.5000f, 0.0f, 0.7f, 0.0f, 1.0f,
           -0.5000f,  0.0000f, 0.0f, 0.7f, 0.0f, 1.0f,
           -0.3536f,  0.3536f, 0.0f, 0.7f, 0.0f, 1.0f
    };

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW
    );

    // Position attribute
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(1.0f, 0.8f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 18);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}