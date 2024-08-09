#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
    // GLFW Initialization
    if(!glfwInit())
    {
        std::cout << "Initialization failed!\n";
        return 1;
    }
    std::cout << "Initialization successful!\n";

    // Window configurations
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window creation 
    GLFWwindow* window = glfwCreateWindow(800, 600, "ISelledMyWifeForInternetConnection", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n" << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cout << "Successfully created GLFW window\n";

    // We tell GLFW to make the context of our window the main context on the current thread.
    glfwMakeContextCurrent(window);

    // Glad Initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << "GLAD Initialization successful! \n";
    
    // Viewport configuration
    glViewport(0, 0, 800, 600);

    // The moment a user resizes the window the viewport should be adjusted as well
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
	return 0;
}

// The moment a user resizes the window the viewport should be adjusted as well. 
// We can register a callback function on the window that gets called each time the window is resized.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}