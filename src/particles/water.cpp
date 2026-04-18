#include "water.h"
#include "grid.h"
#include <random>

// NOTE: Skip acceleration for now

// constexpr int ACCELERATION { 1 };
constexpr int MAX_VEL_Y    { 80 };
constexpr int MAX_VEL_X    { 60 };
constexpr int MIN_VEL_X    { 15 };
constexpr int FRICTION     { 2 };

static std::mt19937 rng { std::random_device{}() };
static std::uniform_int_distribution<int> directionDist{ 0, 1 };

inline int getVelX(Grid &g, int x, int y) { return g.getCellDataX(x, y) - 128; }
inline int getVelY(Grid &g, int x, int y) { return g.getCellDataY(x, y) - 128; }

inline void setVelX(Grid &g, int x, int y, int vx) { g.setCellDataX(x, y, vx); }
inline void setVelY(Grid &g, int x, int y, int vy) { g.setCellDataY(x, y, vy); }

void updateWater(Grid &grid, int x, int y)
{
    // Get the velocity
    int vy { getVelY(grid, x, y) }; 
    int vx { getVelX(grid, x, y) }; 

    // Increase the velocity with ACCELERATION
    // vy = std::min(vy + ACCELERATION, MAX_VEL_Y);
    vy = MAX_VEL_Y;

    // Calculate how many cells the Sand should move this frame
    int stepsY {vy >> VEL_PRECISION_SHIFT };

    // Sub-step by trying to move vy cells downward
    int curX { x };
    int curY { y };
    bool hitGround { false };

    // --- Vertical Movement ---
    for (int step { 0 }; step < std::max(1, stepsY); ++step) {
        // Fall down
        if (grid.isEmpty(curX, curY + 1)) {
            grid.move(curX, curY + 1, curX, curY);
            curY++;
        }
        // Sink through smoke
        else if (grid.getType(curX, curY + 1) == SMOKE) {
            grid.swap(curX, curY + 1, curX, curY);
            curY++;
        }
        // check diagonally if blocked below
        else if (step == 0) {
            int direction { directionDist(rng) ? 1 : -1 };
            if (grid.isEmpty(curX + direction, curY + 1)) {
                grid.move(curX + direction, curY + 1, curX, curY);
                curX += direction;
                curY++;
            }
            else if (grid.isEmpty(curX - direction, curY + 1)) {
                grid.move(curX - direction, curY + 1, curX, curY);
                curX -= direction;
                curY++;
            }
            else {
                hitGround = true;
                // Convert 1/4 of vertical velocity into horizontal on landing
                if (vx == 0) {
                    int gained { vy / 4 };
                    // Randomly decide direction of the velocity
                    vx = directionDist(rng) ? gained : -gained;
                }
                vy = 0;
                break;
            }
        }
         
        else {
            // Stop if it can't move anymore
            hitGround = true;
            if (vx == 0) {
                int gained { vy / 4 };
                // Randomly decide direction of the velocity
                vx = directionDist(rng) ? gained : -gained;
            }
            vy = 0;
            break;
        }
    } 

    // --- Horizontal Movement ---
    if (hitGround) {
        int stepsX { std::abs(vx) >> VEL_PRECISION_SHIFT };
        int dirX { vx > 0 ? 1 : -1 };

        for (int step { 0 }; step < std::max(1, stepsX); ++step) {
            // Check if it can fall diagonally
            if (grid.isEmpty(curX + dirX, curY + 1)) {
                grid.move(curX + dirX, curY + 1, curX, curY);
                curX += dirX;
                curY++;
            }
            // Fall straight down if the diagonal is blocked
            else if (grid.isEmpty(curX, curY + 1)) {
                grid.move(curX, curY + 1, curX, curY);
                curY++;
            }
            // Spread horizontally
            else if (grid.isEmpty(curX + dirX, curY)) {
                grid.move(curX + dirX, curY, curX, curY);
                curX += dirX;
            }
            
            else {
                // Hit a wall or another particle: flip direction and apply friction.
                vx = -vx / FRICTION;
                if (std::abs(vx) < MIN_VEL_X) vx = 0;
                break;
            }
        }
    }
    
    // Write back the final velocity
    setVelY(grid, curX, curY, vy);
    setVelX(grid, curX, curY, vx);
}
