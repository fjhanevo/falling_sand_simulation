// simulation.h
#pragma once

#include "grid.h"
#include "constants.h"
#include "renderer.h"
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
    Particle getSelectedParticle() const { return m_selectedParticle; }
    Renderer &getRenderer() { return m_renderer; }

    void setWidth(int width) { m_width = width; }
    void setHeight(int height) { m_height = height; }
    void setState(State state) { m_state = state; }
    void setSelectedParticle(Particle type) { m_selectedParticle = type; }


    // --- Simulation functions ---
    void processInput();
    void update(float dt);
    void render();

    // --- Public member variables ---
    bool m_mouse_btn_left { false };
    int m_mouseX { 0 };
    int m_mouseY { 0 };
    // TODO: maybe add keyboard variables

private:
    int m_width{};
    int m_height{};
    float m_moveTimer {};
    Particle m_selectedParticle { SAND };
    State m_state{ ACTIVE };
    Grid m_grid{ GRID_WIDTH, GRID_HEIGHT };
    Renderer m_renderer { GRID_WIDTH, GRID_HEIGHT };
    //TODO: add UI element
};
