#pragma once
#include <cstdint>

enum Particle : uint8_t
{
    EMPTY = 0,        // 00000000
    SAND,             // 00000001
    WATER,            // 00000010
    WOOD,             // 00000011
    FIRE,             // 00000100
    SMOKE,            // 00000101
    STONE,            // 00000110
};
