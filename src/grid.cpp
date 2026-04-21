#include "grid.h"
#include "particle.h"
#include "particle_registry.h"
#include <algorithm>
#include <cstdint>
#include <utility>

Grid::Grid(int width, int height)
    : m_width(width), m_height(height)
{
    // Initialize all Cells to EMPTY
    m_cells.resize(m_width * m_height, EMPTY);
}

Particle Grid::getType(int x, int y) const
/*
Get the Particle type at the current index in m_cells
*/
{
    if (!inBounds(x, y)) return EMPTY;
    return (Particle)(m_cells[getIndex(x,y)] & TYPE_MASK);
}

void Grid::setType(int x, int y, Particle type)
/*
Set the current index to a given type
Clearing a cell also wipes the data bytes so no
stale velocity/lifetime state leaks into the next frame 
*/
{
    uint32_t& cell { m_cells[getIndex(x, y)] };
    if (type == EMPTY) {
        cell = EMPTY;
        return;
    }
    cell = (cell & ~TYPE_MASK) | (uint32_t)(type);
}

bool Grid::isUpdated(int x, int y) const
/* 
Check if the cell is updated using the AND operator
*/
{
    return (m_cells[getIndex(x, y)] & UPDATE_FLAG);
}

bool Grid::isFlammable(int x, int y) const
/*
Check if the cell is flammable
*/
{
    return (m_cells[getIndex(x, y)] & FLAMMABLE);
}

void Grid::setUpdated(int x, int y, bool value)
/* 
Toggles the updated status for a cell
*/
{
    if (value)
    {
        // Set the update switch on
        m_cells[getIndex(x, y)] |= UPDATE_FLAG;
    }
    else 
    {
        // Turn off the update swtich 
        m_cells[getIndex(x, y)] &= ~UPDATE_FLAG;    
    }
}

void Grid::setFlammable(int x, int y, bool value)
/*
*/
{
    m_cells[getIndex(x, y)] |= FLAMMABLE;
}

int Grid::getCellDataX(int x, int y) const 
{
    if (!inBounds(x, y)) return 0;
    // Get the cell
    uint32_t cell { m_cells[getIndex(x, y)] };
    // Get the byte and subtract the bias to get a number between -128 and 127
    return (int)((cell & VEL_X_MASK) >> VEL_X_SHIFT);
}

int Grid::getCellDataY(int x, int y) const 
{
    if (!inBounds(x, y)) return 0;
    // Get the cell
    uint32_t cell { m_cells[getIndex(x, y)] };
    // Get the byte and subtract the bias to get a number between -128 and 127
    return (int)((cell & VEL_Y_MASK) >> VEL_Y_SHIFT);}

void Grid::setCellDataX(int x, int y, int val)
{
    if (!inBounds(x, y)) return;
    // Clamp to a valid range before storing (-128 to 127)
    uint32_t vel { (uint32_t)(std::clamp(val, -128, 127) + VEL_BIAS) & 0xFF };
    // Get the cell
    uint32_t &cell { m_cells[getIndex(x, y)] };
    // Clean the cell and apply the new velocity
    cell = (cell & ~VEL_X_MASK) | (vel << VEL_X_SHIFT);
}

void Grid::setCellDataY(int x, int y, int val)
{
    if (!inBounds(x, y)) return;
    // Clamp to a valid range before storing (-128 to 127)
    uint32_t vel { (uint32_t)(std::clamp(val, -128, 127) + VEL_BIAS) & 0xFF };
    // Get the cell
    uint32_t &cell { m_cells[getIndex(x, y)] };
    // Clean the cell and apply the new value 
    cell = (cell & ~VEL_Y_MASK) | (vel << VEL_Y_SHIFT);
}

void Grid::move(int toX, int toY, int fromX, int fromY)
/* 
Moves a given cell to a new position. Marks the old position as EMPTY
*/
{
    // get the cell
    uint32_t cell { m_cells[getIndex(fromX, fromY)] };
    // Move the cell to the new position
    m_cells[getIndex(toX, toY)] = cell;
    // Mark the old position as EMPTY
    m_cells[getIndex(fromX, fromY)] = EMPTY;
    // Set the cell as updated
    setUpdated(toX, toY, true);
}

void Grid::swap(int toX, int toY, int fromX, int fromY)
/*
Swap two cells and mark them as being updated
*/
{
    std::swap(m_cells[getIndex(fromX,fromY)], m_cells[getIndex(toX, toY)]);
    setUpdated(toX, toY, true);
    setUpdated(fromX, fromY, true);
}

bool Grid::isEmpty(int x, int y) const
/*
Check if the cell is empty or if it contains Smoke
*/
{
    if (!inBounds(x, y)) return false;
    Particle type { (Particle)(m_cells[getIndex(x, y)] & TYPE_MASK) };
    return type == EMPTY || type == SMOKE;
}

void Grid::resetUpdateFlags()
/*
Resets the update flag for each cell (sets the 8th bit to 0)
*/
{
    for (uint32_t& cell : m_cells)
        cell &= ~UPDATE_FLAG;
}

void Grid::update()
{
    // Reset update flags
    this->resetUpdateFlags();
    // Loop through the grid in a bottom-up manner
    for (int y { m_height - 1}; y >= 0; y--) {
        for (int x { 0 }; x < m_width; x++) {
            updateParticle(*this, x, y);
        }
    }
}
