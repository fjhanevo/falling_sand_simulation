#include "simulation.h"
#include "constants.h"
#include "particle.h"
#include <GLFW/glfw3.h>
#include <random>

static std::mt19937 rng { std::random_device{}() };

constexpr float MOVE_INTERVAL { 0.006f };

Simulation::Simulation(int width, int height)
    : m_width(width), m_height(height), m_moveTimer(0.0f)
{
}

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

            // Loop through the mouse radius
            for (int dy { -m_brushSize }; dy <= m_brushSize; ++dy) {
                for (int dx { -m_brushSize }; dx <= m_brushSize; ++dx) {
                    // check if the coords are inside the circle
                    if (dx * dx + dy * dy <= m_brushSize * m_brushSize) {
                        int nx = gridX + dx;
                        int ny = gridY + dy;
                        // Check if the particle is EMPTY, then clear the cell
                        if (m_selectedParticle == EMPTY) {
                            if (m_grid.getType(nx, ny) != EMPTY) m_grid.setType(nx, ny, EMPTY);
                        }
                        // Check if the position is valid and the cell is empty
                        else if (m_grid.isEmpty(nx, ny)) {
                            // Randomly spread the particles around the radius
                            if (std::uniform_int_distribution<int> {0, 100}(rng) < 30 ) {
                                m_grid.setType(nx, ny, m_selectedParticle);

                            }
                        }
                    }
                }
            }
        }
        // TODO: Add UI support  
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
    // convert cursor coords to grid coordinates for the brush highlight
    int gridDisplayWidth { m_width - RIGHT_PANEL_PX };
    int gridDisplayHeight { m_height - BOTTOM_PANEL_PX };

    int bx { -1 }, by { -1 }, br { 0 };
    if (m_mouseX >= 0 && m_mouseX < gridDisplayWidth &&
        m_mouseY >= 0 && m_mouseY < gridDisplayHeight) {
        bx = (int)((float)m_mouseX / gridDisplayWidth  * GRID_WIDTH);
        by = (int)((float)m_mouseY / gridDisplayHeight * GRID_HEIGHT);
        br = m_brushSize;
    }
    m_renderer.drawGrid(m_grid, bx, by, br);
}
