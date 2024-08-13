#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

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

void errorCheck(GLuint shader, GLenum type)
{
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (type == GL_VERTEX_SHADER)
    {
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    else if (type == GL_LINK_STATUS)
    {
        if (!success)
        {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
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
    std::cout << "Initialization successful!\n";

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
    std::cout << "Successfully created GLFW window!\n";

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
    std::cout << "GLAD Initialization successful! \n";
#pragma endregion

#pragma region Build and Compile the Shader Program
    // Create an unique ID corresponding to Vertex Shader
    GLuint vertexShader;

    // Generate a vertex shader and bind with ID.
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach the shader source code to the shader object 
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    // Compile the shader 
    glCompileShader(vertexShader);

    // Error checking of the vertex shader compilation.
    errorCheck(vertexShader, GL_VERTEX_SHADER);
    std::cout << "Compilation of the vertex shader is successful!\n";

    // Create an unique ID corresponding to Fragment Shader
    GLuint fragmentShader;

    // Generate fragment shader and bind with ID.
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach the shader source code to the shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    // Error checking of the fragment shader compilation.
    errorCheck(fragmentShader, GL_FRAGMENT_SHADER);
    std::cout << "Compilation of the fragment shader is successful!\n";

    // Create an unique ID corresponding to Shader Program
    GLuint shaderProgram;

    // Generate shader program and bind with ID
    shaderProgram = glCreateProgram();

    // Attach the previously compiled shaders to the program object and then link them.
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Error checking for the linking process of the shader program.
    errorCheck(shaderProgram, GL_LINK_STATUS);
    std::cout << "Linking of the shader program is successful!\n";

    // Delete the shader objects once we've linked them into the program object
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
#pragma endregion

#pragma region Vertex Manipulation
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    // Positions of my first Rectangle
    float vertices[] {
    -0.5f, -0.5f, 0.0f, // bottom left
     0.5f, -0.5f, 0.0f, // bottom right
     -0.5f, 0.5f, 0.0f, // top left
     0.5f, 0.5f, 0.0f, // top right
    };

    // Specify indices of unique vertices to draw them as a rectangle
    unsigned int indices[] {
        0, 1, 2, // first triangle
        2, 3, 1 // second triangle
    };

    // Create an unique ID corresponding to VBO (Vertex Buffer Object)
    GLuint VBO;

    // Generate one buffer object.
    glGenBuffers(1, &VBO);

    // Bind the created buffer object above to the buffer type of VBO which is GL_ARRAY_BUFFER.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Lets fill this VBO.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create an unique ID corresponding to EBO (Element Buffer Object)
    GLuint EBO;

    // Generate one element buffer object
    glGenBuffers(1, &EBO);

    //Bind buffer with type of EBO which is GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Copy the indices into the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Create an unique ID corresponding to VAO (Vertex Array Object) 
    GLuint VAO;

    // Generate one vertex array object.
    glGenVertexArrays(1, &VAO);

    // Bind Vertex Array Object
    glBindVertexArray(VAO);

    // Set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Any subsequent drawing calls will render the triangles in wireframe mode 
    // until we set it back to its default using glPolygonMode(GL_FRONT_AND_BACK, GL_FILL).
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

        glUseProgram(shaderProgram);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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
    std::cout << "VAO(s) deleted!\n";
    glDeleteBuffers(1, &VBO);
    std::cout << "VBO(s) deleted!\n";
    glDeleteProgram(shaderProgram);
    std::cout << "Shader program deleted!\n";

    //Although every non-destroyed windows will be closed when glfwTerminate called, I will call destroy window for clarification.
    glfwDestroyWindow(window);
    std::cout << "Window destroyed!\n";

    // GLFW: Terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    std::cout << "GLFW Terminated!\n";

	return 0;
#pragma endregion
}

