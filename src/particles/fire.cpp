// fire.cpp
#include "fire.h"
#include "grid.h"
#include "particle.h"
#include <random>

constexpr int MIN_LIFETIME { 30 };
constexpr int MAX_LIFETIME { 90 };

static std::mt19937 rng { std::random_device{}() };
static std::uniform_int_distribution<int> lifetimeDist { MIN_LIFETIME, MAX_LIFETIME };
static std::uniform_int_distribution<int> fireDist { 0, 100 };  
static std::uniform_int_distribution<int> neighborCheck {0,3};

// Reuse the y-velocity bits as the lifetime counter
inline int  getLifetime(const Grid &g, int x, int y)      { return g.getCellDataY(x, y); }
inline void setLifetime(Grid &g, int x, int y, int life) { g.setCellDataY(x, y, life - VEL_BIAS); }


int getFireLifetime(const Grid &grid, int x, int y) { return getLifetime(grid, x, y); }

void updateFire(Grid &grid, int x, int y)
{
    // Generate a lifetime if it doesn't have any 
    int lifetime { getLifetime(grid, x, y) };
    if (lifetime == 0) {
        lifetime = lifetimeDist(rng);
        setLifetime(grid, x, y, lifetime);
    }

    bool nearFuel  { false };
    bool nearWater { false };
    bool checkFire { fireDist(rng) < 40 };
    bool catchFire { fireDist(rng) < 40 };
    bool spawnSmoke { fireDist(rng) < 40 };
    bool killFire { fireDist(rng) < 40 };
    int fireCount { 0 };

    // Check neighbouring cells
    for (int dx { -1 }; dx <= 1; dx++) {
        for (int dy { -1 }; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            Particle p = grid.getType(x + dx, y + dy);
            if (p == WATER) nearWater = true;
            if (p == FIRE)  fireCount++;
            if (p != EMPTY && grid.isFlammable(x + dx, y + dy) && !(p == FIRE)) nearFuel = true;
        }
    }

    // Turn to Smoke instanlty if near Water
    if (nearWater) {
        grid.setType(x, y, SMOKE);
        return;
    }

    // Adjust lifetime based on if the Fire is near fuel or not
    if (nearFuel) lifetime--;
    else lifetime -= 3;

    // Check Fire spreading
    if (checkFire) {
        int rx { x + neighborCheck(rng) - 1 };
        int ry { y + neighborCheck(rng) - 1 };

        Particle p { grid.getType(rx, ry) };
        
        if (p != EMPTY && grid.isFlammable(rx, ry) && !(p == FIRE)) {
            if (catchFire) grid.setType(rx, ry, FIRE);
        }
    }

    // Lifetime and Smoke interactions
    if (lifetime <= 0 || (fireCount < 1 && !nearFuel && killFire))
        grid.setType(x, y, SMOKE);
    else if (grid.isEmpty(x, y - 1) && spawnSmoke)
        grid.setType(x, y - 1, SMOKE);

    // Write back the state
    setLifetime(grid, x, y, lifetime);
}
