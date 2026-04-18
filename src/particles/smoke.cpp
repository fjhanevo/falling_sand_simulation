#include "smoke.h"
#include "grid.h"
#include <algorithm>
#include <random>

constexpr int MAX_VEL      { 80 };
constexpr int MIN_VEL      { 40 };
constexpr int MIN_LIFETIME { 30 };
constexpr int MAX_LIFETIME { 80 };

static std::mt19937 rng { std::random_device{}() };
static std::uniform_int_distribution<int> directionDist { 0, 1 };
static std::uniform_int_distribution<int> velocityDist  { MIN_VEL, MAX_VEL };
static std::uniform_int_distribution<int> lifetimeDist  { MIN_LIFETIME, MAX_LIFETIME };

// Smoke reuses the Y-velocity byte (bits 24-31) as its lifetime counter.
// setCellDataY applies +VEL_BIAS on write while getCellDataY returns the raw
// byte, so writing (life - VEL_BIAS) lets getCellDataY read back `life`.
inline int  getLifetime(const Grid& g, int x, int y)           { return g.getCellDataY(x, y); }
inline void setLifetime(Grid& g, int x, int y, int life)       { g.setCellDataY(x, y, life - VEL_BIAS); }

// Use the Velocity x-bits for general velocty
inline void setVel(Grid &g, int x, int y, int vx) { g.setCellDataX(x, y, vx); }

int getSmokeLifetime(const Grid& grid, int x, int y) { return getLifetime(grid, x, y); }

void updateSmoke(Grid& grid, int x, int y)
{
    // Lazy-init lifetime the first time a freshly placed smoke cell ticks
    int lifetime { getLifetime(grid, x, y) };
    if (lifetime == 0) {
        lifetime = lifetimeDist(rng);
        setLifetime(grid, x, y, lifetime);
    }

    // Age one tick, disappear when spent
    if (--lifetime <= 0) {
        grid.setType(x, y, EMPTY);
        return;
    }

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

    // Write state back at the final position
    setVel(grid, curX, curY, vel);
    setLifetime(grid, curX, curY, lifetime);
}
