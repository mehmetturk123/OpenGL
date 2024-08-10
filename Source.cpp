#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
    // In case a GLFW function fails, an error is reported to the GLFW error callback
    glfwSetErrorCallback(error_callback);

    // GLFW Initialization
    if(!glfwInit())
    {
        std::cout << "Initialization failed!\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "Initialization successful!\n";

    // Window configurations
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window creation 
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

    // Glad Initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "GLAD Initialization successful! \n";
    
    // Viewport configuration
    glViewport(0, 0, 800, 600);

    // The moment a user resizes the window the viewport should be adjusted as well
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        //rendering
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Although every non-destroyed windows will be closed when glfwTerminate called, I will call destroy window for clarification.
    glfwDestroyWindow(window);
    std::cout << "Window destroyed!\n";

    glfwTerminate();
    std::cout << "GLFW Terminated!\n";
	return 0;
}

