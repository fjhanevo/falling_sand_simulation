#include "simulation.h"

Simulation::Simulation(int width, int height)
    : m_width(width), m_height(height), m_grid(m_width, m_height)
{}

void Simulation::runSimulation()
{
    // Loop through the grid in a bottom-up manner
    for (int y = m_height - 1; y >= 0; --y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            //TODO: Implement Simulation logic!
            
            // Clear all the update flags
            m_grid.resetUpdateFlags();
        }
    }
}
