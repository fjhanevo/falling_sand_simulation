#include "sand.h"
#include <random>

static std::mt19937 rng { std::random_device{}() };
static std::uniform_int_distribution<int> direction{ 0, 1 };


void updateSand(Grid& grid, int x, int y)
{
    // Fall down
    if (grid.isEmpty(x, y + 1))
        grid.move(x, y + 1, x, y);
    // check diagonally
    else {
        int dir { direction(rng) ? 1 : -1 };
        if (grid.isEmpty(x + dir, y + 1))
            grid.move(x + dir, y + 1, x, y);
        else if (grid.isEmpty(x - dir, y + 1))
            grid.move(x - dir, y + 1, x, y);
    }
} 
