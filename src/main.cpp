#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Forward declare callback functions
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// Set display dimensions
constexpr int SCREEN_HEIGHT { 800 };
constexpr int SCREEN_WIDTH { 600 };

int main()
{
    // Initialize window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Create the window
    GLFWwindow *window {glfwCreateWindow(
        SCREEN_WIDTH, 
        SCREEN_HEIGHT,
        "Falling Sand Simulation", nullptr, nullptr
    )};

    glfwMakeContextCurrent(window);

    // ----- Load all OpenGL function pointers -----
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //TODO: Create a Game object here

    // Set callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //NOTE: Add glEnable(GL_BLEND) and blendFunc here if needed

    // ----- Main render loop -----
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ||
        key == GLFW_KEY_Q && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0,0, width, height);
}
