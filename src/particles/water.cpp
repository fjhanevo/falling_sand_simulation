#include "water.h"

#include <random>


static std::mt19937 rng { std::random_device{}() };
static std::uniform_int_distribution<int> directionDist{ 0, 1 };

void updateWater(Grid &grid, int x, int y)
{
    int nextY = y + 1;
    int direction { directionDist(rng) ? 1 : -1 };
    // Fall down
    if (grid.isEmpty(x, nextY)) 
        grid.move(x, nextY, x, y);
    else if (grid.isEmpty(x + direction, nextY))
        grid.move(x + direction, nextY, x, y);
    else if (grid.isEmpty(x + direction, y))
        grid.move(x + direction, y, x, y);

    // check horizontally
    else if (grid.isEmpty(x - direction, nextY)) {
        // flip the direction
        direction = -direction;
        grid.move(x + direction, nextY, x, y);
    }
    else if (grid.isEmpty(x - direction, y)) {
        // flip the direction
        direction = -direction;
        grid.move(x + direction, y, x, y);
    }
}
