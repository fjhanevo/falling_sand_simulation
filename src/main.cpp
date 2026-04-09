#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Constants ---
// display dimensions
constexpr int SCREEN_HEIGHT { 1000 };
constexpr int SCREEN_WIDTH { 800 };
// grid dimensions
constexpr int GRID_HEIGHT { 800 };
constexpr int GRID_WIDTH { 600 };

// Forward declare callback functions
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void mouse_callback(GLFWwindow *window, int button, int action, int mods);

// Cursor declarations
double g_mouseX { 0.0 }, g_mouseY { 0.0 };
bool l_mouse_button = false;

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

    // --- Load all OpenGL function pointers ---
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //TODO: Create a Game object here

    // Set callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glViewport(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //NOTE: Add glEnable(GL_BLEND) and blendFunc here if needed

    // ----- Main render loop -----
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
        if (l_mouse_button) {
            int gridX { (int) (g_mouseX / SCREEN_WIDTH * GRID_WIDTH) };
            int gridY { (int) (g_mouseY / SCREEN_HEIGHT * GRID_HEIGHT) };
            //TODO: replace below with particle adding function
            std::cout << "Cursor position = (" << gridX << ", " << gridY << ")\n";
        }
    }
    glfwTerminate();

    return 0;
}

// --- Callback functions ---
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

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    g_mouseX = xpos;
    g_mouseY = ypos;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) l_mouse_button = true;
        else if (action == GLFW_RELEASE) l_mouse_button = false;
    }
    
}
