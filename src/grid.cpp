#include "grid.h"

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
    return (Particle)(m_cells[getIndex(x,y)] & TYPE_MASK);
}

void Grid::setType(int x, int y, Particle type)
/*
Set the Particle the the current index to a given type.
*/
{
    // Get the cell 
    uint32_t& cell { m_cells[getIndex(x, y)] };
    // Update the cell using the OR operator
    cell = (cell & ~TYPE_MASK) | (uint32_t)(type);
}

bool Grid::isUpdated(int x, int y) const
/* 
Check if the cell is updated using the AND operator
*/
{
    return (m_cells[getIndex(x, y)] & UPDATE_FLAG);
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
        // Only turn off the update swtich (bit 8)
        m_cells[getIndex(x, y)] &= ~UPDATE_FLAG;    
    }
}

void Grid::move(int fromX, int fromY, int toX, int toY)
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
}

void Grid::resetUpdateFlags()
/*
Resets the update flag for each cell (sets the 8th bit to 0)
*/
{
    for (uint32_t& cell : m_cells)
        cell &= ~UPDATE_FLAG;
}
