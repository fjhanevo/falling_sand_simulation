#include "simulation.h"
#include "constants.h"
#include <GLFW/glfw3.h>

constexpr float MOVE_INTERVAL { 0.006f };

Simulation::Simulation(int width, int height)
    : m_width(width), m_height(height), m_moveTimer(0.0f)
{}

void Simulation::processInput()
{
    if (m_mouse_btn_left) {
        // check if the click is inside the grid region
        int gridDisplayWidth { m_width - RIGHT_PANEL_PX };
        int gridDisplayHeight { m_height - BOTTOM_PANEL_PX };

        if (m_mouseX >= 0 && m_mouseX < gridDisplayWidth &&
            m_mouseY >= 0 && m_mouseY < gridDisplayHeight) {
            float normX { (float)m_mouseX / (float)gridDisplayWidth  };
            float normY { (float)m_mouseY / (float)gridDisplayHeight };

            int gridX { (int)(normX * GRID_WIDTH)  };
            int gridY { (int)(normY * GRID_HEIGHT) };

            m_grid.setType(gridX, gridY, m_selectedParticle);
        }
        // TODO: Add else option for UI click
    }
}

void Simulation::update(float dt)
{
    if (m_state == ACTIVE) {
        // increment the moveTimer
        m_moveTimer += dt;    
        
        // check if enough time has passed
        if (m_moveTimer >= MOVE_INTERVAL) {
            // update the grid and reset the moveTimer
            m_grid.update();
            m_moveTimer = 0.0f;
        }
    }
}

void Simulation::render()
{
    m_renderer.drawGrid(m_grid);
}
