#include "particle_registry.h"
#include "grid.h"
#include "particle.h"
#include "sand.h"
#include "water.h"
#include "wood.h"
#include <array>

using UpdateFn = void(*)(Grid&, int, int);

static const UpdateFn s_updateTable[] = {
    nullptr,        // EMPTY
    updateSand,     // SAND
    updateWater,    // WATER
    updateWood,     // WOOD

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

};

std::array<uint8_t, 3> getParticleColor(Particle type)
{
    return { s_colorTable[type][0], 
             s_colorTable[type][1], 
             s_colorTable[type][2]
    };
}
