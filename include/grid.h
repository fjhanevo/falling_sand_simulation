#pragma once
#include <cstdint>
#include <vector>
// Explicitly use 8 bits
// Can add up to 255 Particles
enum Particle : uint8_t
{
    EMPTY = 0,              // 00000000
    SAND,                   // 00000001
    WATER,                  // 00000010
    FIRE,                   // 00000011
};

// ----- Bit Masks -----
constexpr uint32_t TYPE_MASK    { 0xFF };      // Hex for 255
constexpr uint32_t UPDATE_FLAG  { (1 << 8) };  // Shift left 8 times

class Grid
{
public:
    // Constructor 
    Grid(int width, int height);

    // ----- Member functions -----
    Particle getType(int x, int y) const;
    void setType(int x, int y, Particle type);

    bool isUpdated(int x, int y) const;
    void setUpdated(int x, int y, bool value);

    void move(int fromX, int fromY, int toX, int toY);
    void resetUpdateFlags();

    // Get the raw data from m_cells for drawing and rendering
    const uint32_t* getCellData() const { return m_cells.data(); }

private:
    int m_width{}, m_height{};
    std::vector<uint32_t> m_cells{};

    // Internal helper function to get the index of a cell
    int getIndex(int x, int y) const { return x + y * m_width; }
};
