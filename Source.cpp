#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image.h"
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
    
    // Vertex data for Rectangle
     float vertices[] = {
          // positions         // colors           // texture coords
          0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
          0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
         -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
         -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
     };
     
     unsigned int indices[]{ // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
     };

     unsigned int VAO, VBO, EBO;
     glGenVertexArrays(1, &VAO);
     glGenBuffers(1, &VBO);
     glGenBuffers(1, &EBO);

     glBindVertexArray(VAO);
     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
     glEnableVertexAttribArray(2);

#pragma endregion
#pragma region Texture Manipulation

    unsigned int textures[2];
    // First Texture 
    //---------------
    glGenTextures(2, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("C:\\Users\\mailt\\OneDrive\\Resimler\\Textures\\container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Second Texture
    //---------------
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("C:\\Users\\mailt\\OneDrive\\Resimler\\Textures\\awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    ourShader.use(); // don't forget to activate the shader before setting uniforms!  
    // We also have to tell OpenGL to which texture unit each shader sampler belongs to
    ourShader.setInt("texture1", 0); 
    ourShader.setInt("texture2", 1);


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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
    glDeleteBuffers(1, &EBO);

    glDeleteProgram(ourShader.ID);

    //Although every non-destroyed windows will be closed when glfwTerminate called, I will call destroy window for clarification.
    glfwDestroyWindow(window);

    // GLFW: Terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();

	return 0;
#pragma endregion
}

