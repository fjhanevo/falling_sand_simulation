#pragma once
#include "grid.h"

class Simulation
{
public:
    Simulation(int width, int height);

    void runSimulation();
    // ----- Implement functions for how different Particles should behave -----

private:
    int m_width{}, m_height{};
    Grid m_grid;
};

