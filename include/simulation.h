// simulation.h
#pragma once

#include "grid.h"
#include "constants.h"
#include <GLFW/glfw3.h>

enum State { ACTIVE, PAUSED };

class Simulation
{
public:
    // Constructor 
    Simulation(int width, int height);
    
    // --- Getters/setters ---
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    State getState() const { return m_state; }
    void setState(State state) { m_state = state; }
    void setWidth(int width) { m_width = width; }
    void setHeight(int height) { m_height = height; }

    // --- Simulation functions ---
    void processInput();
    void update();
    void render();

    // --- Public member variables ---
    bool m_mouse_btn_left { false };
    int m_mouseX { 0 };
    int m_mouseY { 0 };
    // TODO: maybe add keyboard variables

private:
    int m_width{};
    int m_height{};
    State m_state{ ACTIVE };
    Grid m_grid{ GRID_WIDTH, GRID_HEIGHT };
    //TODO: add UI element
};
