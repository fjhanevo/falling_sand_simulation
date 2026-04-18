#include "wood.h"

void updateWood(Grid &grid, int x, int y)
{
    grid.setFlammable(x, y, true);
}
