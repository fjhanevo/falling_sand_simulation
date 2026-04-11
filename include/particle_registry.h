// particle_registry.h
#pragma once
#include "grid.h"
#include "particle.h"
#include <array>

void updateParticle(Grid& grid, int x, int y);
std::array<uint8_t, 3> getParticleColor(Particle type);
