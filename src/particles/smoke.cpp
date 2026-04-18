#include "smoke.h"
#include "grid.h"
#include <random>

constexpr int MAX_VEL { 80 };
constexpr int MIN_VEL { 40 };

static std::mt19937 rng { std::random_device{}() };
static std::uniform_int_distribution<int> directionDist{ 0, 1 };
static std::uniform_int_distribution<int> velocityDist{ MIN_VEL, MAX_VEL };


// Use the Velocity x-bits for general velocty
inline void setVel(Grid &g, int x, int y, int vx) { g.setCellDataX(x, y, vx); }

void updateSmoke(Grid& grid, int x, int y)
{
    // Randomly generate a velocity
    int vel { velocityDist(rng) };

    // Calculate how many cells to move
    int steps { vel >> VEL_PRECISION_SHIFT };

    // sub-stepping coords
    int curX { x };
    int curY { y };

    for (int step { 0 }; step < std::max(1, steps); ++step) {
        // Rise up 
        int dirX { directionDist(rng) ? 1 : -1 };
        if (grid.isEmpty(curX, curY - 1)) {
            grid.move(curX, curY - 1, curX, curY);
            curY--;
        }

        // Drift sideways while rising
        if (grid.isEmpty(curX + dirX, curY - 1)) {
            grid.move(curX + dirX, curY - 1, curX, curY);
            curY--;
            curX += dirX;
        }
        // Drift horizontally if it can't move upwards anymore
        else if (grid.isEmpty(curX + dirX, curY)) {
            grid.move(curX + dirX, curY, curX, curY);
            curX += dirX;
        }
    }

    // Report back the velocity
    setVel(grid, curX, curY, vel);
}
