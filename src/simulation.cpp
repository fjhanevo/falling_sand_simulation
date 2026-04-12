#include "simulation.h"
#include "constants.h"
#include <iostream>

Simulation::Simulation(int width, int height)
    : m_width(width), m_height(height)
{}

void Simulation::processInput()
{
    if (m_mouse_btn_left) {
        // check if the click is inside the grid region
        if (m_mouseX < GRID_PIXEL_W && m_mouseY < GRID_PIXEL_H) {
            int gridX { (int) (m_mouseX / CELL_SIZE) };
            int gridY { (int) (m_mouseY / CELL_SIZE) };
            m_grid.setType(gridX, gridY, m_selectedParticle);
            std::cout << gridX << ", " << gridY << '\n';

        }
        // TODO: Add else option for UI click
    }
}

void Simulation::render()
{
    m_renderer.drawGrid(m_grid);
}
