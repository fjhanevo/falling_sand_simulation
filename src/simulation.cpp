#include "simulation.h"
#include "constants.h"

Simulation::Simulation(int width, int height)
    : m_width(width), m_height(height)
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
        // if (m_mouseX < GRID_PIXEL_W && m_mouseY < GRID_PIXEL_H) {
        //     int gridX { (int) (m_mouseX / CELL_SIZE) };
        //     int gridY { (int) (m_mouseY / CELL_SIZE) };
        //     m_grid.setType(gridX, gridY, m_selectedParticle);
        //
        // }
        // TODO: Add else option for UI click
    }
}

void Simulation::render()
{
    m_renderer.drawGrid(m_grid);
}
