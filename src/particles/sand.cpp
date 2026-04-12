#include "sand.h"

void updateSand(Grid& grid, int x, int y)
{
    // Fall down
    if (grid.isEmpty(x, y - 1))
        grid.move(x, y - 1, x, y);
} 
