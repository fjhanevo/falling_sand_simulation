#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "constants.h"
#include "simulation.h"

// Forward declare callback functions
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

int main()
{
    // --- Initialize window ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // --- Create the window ---
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

    Simulation sim { SCREEN_WIDTH, SCREEN_HEIGHT };
    glfwSetWindowUserPointer(window, &sim);


    // --- Set callback functions ---
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glViewport(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // --- Delta time ---
    float deltaTime { 0.0f };
    float lastFrame { 0.0f };

    // --- Main render loop ---
    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        glfwPollEvents();
        sim.processInput();

        // Update the Simulation
        sim.update(deltaTime);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sim.render();
        glfwSwapBuffers(window);
    }
    glfwTerminate();

    return 0;
}

// --- Callback functions ---
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    // Get the Simulation instance
    Simulation *sim { static_cast<Simulation*>(glfwGetWindowUserPointer(window)) };
    if (!sim) return;
    
    if (action == GLFW_PRESS) {
        switch (key) { 
            case GLFW_KEY_ESCAPE: 
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, true);
                break;
            case GLFW_KEY_1:
                sim->setSelectedParticle(SAND);
                break;
            case GLFW_KEY_2:
                sim->setSelectedParticle(WATER);
                break;
        }
    }
    
}
static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Get the Simulation instance
    Simulation *sim { static_cast<Simulation*>(glfwGetWindowUserPointer(window)) };
    if (!sim) return;
    sim->setWidth(width);
    sim->setHeight(height);
    sim->getRenderer().buildQuad(width, height);
    glViewport(0,0, width, height);

}

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    // Get the Simulation instance
    Simulation *sim { static_cast<Simulation*>(glfwGetWindowUserPointer(window)) };
    if (!sim) return;
    sim->m_mouseX = xpos;
    sim->m_mouseY = ypos;
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    // Get the Simulation instance
    Simulation *sim { static_cast<Simulation*>(glfwGetWindowUserPointer(window)) };
    if (!sim) return;
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) sim->m_mouse_btn_left = true;
        else if (action == GLFW_RELEASE) sim->m_mouse_btn_left = false;
    }
}
