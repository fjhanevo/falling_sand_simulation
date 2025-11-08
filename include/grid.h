#pragma once
#include <cstdint>
#include <vector>
// Explicitly use 8 bits
// Can add up to 255 Particles
enum Particle : uint8_t
{
    EMPTY = 0,
    SAND,
    WATER,
};

class Grid
{
public:
    Grid(int width, int height);
    ~Grid();


private:
    int m_width{}, m_height{};
    std::vector<uint32_t> m_cells{};
};
