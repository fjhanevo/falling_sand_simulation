#include "particle_registry.h"
#include "grid.h"
#include "particle.h"
#include "sand.h"
#include "water.h"
#include "wood.h"
#include "fire.h"
#include "smoke.h"
#include "stone.h"
#include <algorithm>
#include <array>
#include <cstdint>

using UpdateFn = void(*)(Grid&, int, int);

static const UpdateFn s_updateTable[] = {
    nullptr,        // EMPTY
    updateSand,     // SAND
    updateWater,    // WATER
    updateWood,     // WOOD
    updateFire,     // FIRE 
    updateSmoke,    // SMOKE
    updateStone,    // STONE
};

void updateParticle(Grid &grid, int x, int y)
{
    Particle type { grid.getType(x, y) };
    if (!grid.isUpdated(x, y) && s_updateTable[type])
        s_updateTable[type](grid, x, y);
}

// --- Color table ---
static const uint8_t s_colorTable[][3] = {
//     R    G    B
    {  0,   0,   0  },  // EMPTY
    { 246, 215, 176 },  // SAND
    {  35, 137, 218 },  // WATER
    { 139, 105,  20 },  // WOOD
    { 139, 105,  20 },  // WOOD
    {  43,  43,  42},   // STONE
};

std::array<uint8_t, 3> getParticleColor(const Grid& grid, int x, int y)
{
    Particle type { grid.getType(x, y) };

    if (type == SMOKE) {
        int lifetime { getSmokeLifetime(grid, x, y) };
        if (lifetime == 0) lifetime = 80;
        float t { lifetime / 100.0f };
        uint8_t v { (uint8_t)std::clamp((int)(t * 0.5f * 255.0f), 0, 255) };
        return { v, v, v };
    }
    else if (type == FIRE) {
        int lifetime { getFireLifetime(grid, x, y) };
        if (lifetime == 0) lifetime = 80;
        float t { lifetime / 80.0f };
        uint8_t g { (uint8_t)std::clamp((int)(t * 0.8f * 255.0f), 0, 255) };
        return { 255, g, 0 };
    }

    return { s_colorTable[type][0],
             s_colorTable[type][1],
             s_colorTable[type][2]
    };
}
