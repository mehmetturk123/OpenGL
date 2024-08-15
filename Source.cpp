#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>
#include <filesystem>

GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case 0x0503:                           error = "STACK_OVERFLOW"; break;
        case 0x0504:                           error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

// The moment a user resizes the window the viewport should be adjusted as well. 
// We can register a callback function on the window that gets called each time the window is resized.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// In case a GLFW function fails, an error is reported to the GLFW error callback.
// You can receive these reports with an error callback.
// This function must have the signature below but may do anything permitted in other callbacks.
void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
        std::cout << "Escape key pressed!\n";
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
#pragma region GLFW: Initialize and Configure
    // GLFW Initialization
    if(!glfwInit())
    {
        std::cout << "Initialization failed!\n";
        exit(EXIT_FAILURE);
    }

    // In case a GLFW function fails, an error is reported to the GLFW error callback
    glfwSetErrorCallback(error_callback);

    // Window configurations
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#pragma endregion

#pragma region GLFW Window creation 
    GLFWwindow* window = glfwCreateWindow(800, 600, "I Selled My Wife For Internet Connection", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // We tell GLFW to make the context of our window the main context on the current thread.
    glfwMakeContextCurrent(window);

    // The moment a user resizes the window the viewport should be adjusted as well
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
#pragma endregion

#pragma region Glad Initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
#pragma endregion

#pragma region Build and Compile the Shader Program
     Shader ourShader("vs.vert", "fs.frag");
#pragma endregion

#pragma region Vertex Manipulation
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    // Positions of my first Triangle
    float vertices[] = {
         // positions        // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };

    // Create an unique ID corresponding to VBO (Vertex Buffer Object) buffer
    GLuint VBO;

    // Generate one buffer object.
    glGenBuffers(1, &VBO);

    // Bind the created buffer object above to the buffer type of VBO which is GL_ARRAY_BUFFER.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Lets fill this VBO.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create an unique ID corresponding to VAO (Vertex Array Object) buffer
    GLuint VAO;

    // Generate one vertex array object.
    glGenVertexArrays(1, &VAO);

    // Bind Vertex Array Object
    glBindVertexArray(VAO);

    // Set our vertex attributes pointers
    // ----------------------------------
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
#pragma endregion

#pragma region Render Loop
    // Render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        //rendering
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        ourShader.use();

        float timeValue = glfwGetTime();
        float offsetValue = (sin(timeValue) * 0.5f);
        int vertexXOffsetLocation = glGetUniformLocation(ourShader.ID, "xOffset");
        glUniform1f(vertexXOffsetLocation, offsetValue);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
#pragma endregion

#pragma region Termination
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteProgram(ourShader.ID);

    //Although every non-destroyed windows will be closed when glfwTerminate called, I will call destroy window for clarification.
    glfwDestroyWindow(window);

    // GLFW: Terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();

	return 0;
#pragma endregion
}

