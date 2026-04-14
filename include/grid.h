// grid.h
#pragma once
#include <cstdint>
#include <vector>
#include "particle.h"

// ----- Bit Masks -----
constexpr uint32_t TYPE_MASK    { 0xFF };      // Hex for 255
constexpr uint32_t UPDATE_FLAG  { (1 << 8) };  // Shift left 8 times
// legg til bit masks for Fire og velocity

class Grid
{
public:
    // Constructor 
    Grid(int width, int height);

    // ----- Member functions -----
    Particle getType(int x, int y) const;
    void setType(int x, int y, Particle type);

    bool isUpdated(int x, int y) const;
    bool isEmpty(int x, int y) const;
    void setUpdated(int x, int y, bool value);

    Particle getParticle() const { return m_particle; }
    void setParticle(Particle type) { m_particle = type; }

    void move(int toX, int toY, int fromX, int fromY);
    void swap(int toX, int toY, int fromX, int fromY);
    // void swap(int x1, int y1, int x2, int y2);
    void resetUpdateFlags();
    void update();

    // Get the raw data from m_cells for drawing and rendering
    const uint32_t* getCellData() const { return m_cells.data(); }

private:
    int m_width{}, m_height{};
    Particle m_particle { SAND };
    std::vector<uint32_t> m_cells{};

    // Internal helper function to get the index of a cell
    int getIndex(int x, int y) const { return x + y * m_width; }
    bool inBounds(int x, int y) const { return x >= 0 && x < m_width && y >= 0 && y < m_height; }
};
