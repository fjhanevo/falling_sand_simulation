// grid.h
#pragma once
#include <cstdint>
#include <vector>
#include "particle.h"

// --- Particle and Flags ---
constexpr uint32_t TYPE_MASK           { 0xFF };      // Hex for 255
constexpr uint32_t UPDATE_FLAG         { 1 << 8 };    // Flip the 8th bit 
constexpr uint32_t FLAMMABLE           { 1 << 9 };    // Flip the 9th bit 
// --- Velocity X (Bit 16-23) ---
constexpr uint32_t VEL_X_MASK          { 0xFF0000 };
constexpr int      VEL_X_SHIFT         { 16 };
// --- Velocity Y (Bit 24-31) ---
constexpr uint32_t VEL_Y_MASK          { 0xFF000000 };
constexpr int      VEL_Y_SHIFT         { 24 };
// --- Velocity utils ---
constexpr int      VEL_BIAS            { 128 };        // Default velocity (0)
constexpr int      VEL_PRECISION_SHIFT { 4 };

class Grid
{
public:
    // Constructor 
    Grid(int width, int height);

    // --- Member functions ---
    // Particle getter/setter
    Particle getType(int x, int y) const;
    void setType(int x, int y, Particle type);

    // Flag functions
    bool isUpdated(int x, int y) const;
    bool isFlammable(int x, int y) const;
    void setUpdated(int x, int y, bool value);
    void setFlammable(int x, int y, bool value);

    // Velocity functions
    int getVelX(int x, int y) const;
    int getVelY(int x, int y) const;
    void setVelX(int x, int y, int vx);
    void setVelY(int x, int y, int vx);

    // Position/updating functions
    void move(int toX, int toY, int fromX, int fromY);
    void swap(int toX, int toY, int fromX, int fromY);
    bool isEmpty(int x, int y) const;
    void resetUpdateFlags();
    void update();


private:
    int m_width{}, m_height{};
    std::vector<uint32_t> m_cells{};

    // Internal helper function to get the index of a cell
    int getIndex(int x, int y) const { return x + y * m_width; }
    bool inBounds(int x, int y) const { return x >= 0 && x < m_width && y >= 0 && y < m_height; }
};
