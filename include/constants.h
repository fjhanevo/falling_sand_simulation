// constants.h
#pragma once
// --- Constants ---
// Display dimensions
constexpr int SCREEN_HEIGHT { 1024 };
constexpr int SCREEN_WIDTH  { 768 };
constexpr int CELL_SIZE     { 4 };
// Space for UI panels
constexpr int RIGHT_PANEL_PX  { 160 };
constexpr int BOTTOM_PANEL_PX { 60 };
// Grid dimensions
constexpr int GRID_PIXEL_W  { SCREEN_WIDTH  - RIGHT_PANEL_PX };
constexpr int GRID_PIXEL_H  { SCREEN_HEIGHT - BOTTOM_PANEL_PX };
constexpr int GRID_HEIGHT   { GRID_PIXEL_H / CELL_SIZE };
constexpr int GRID_WIDTH    { GRID_PIXEL_W / CELL_SIZE };

