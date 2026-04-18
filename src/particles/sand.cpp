#include "sand.h"
#include "grid.h"
#include <random>

// NOTE: Skip acceleration for now

// constexpr int ACCELERATION { 2 };   // Acceleration per frame
constexpr int MAX_VEL      { 120 };  

static std::mt19937 rng { std::random_device{}() };
static std::uniform_int_distribution<int> directionDist{ 0, 1 };

inline int getVelY(Grid &g, int x, int y) { return g.getCellDataY(x, y) - 128; }
inline void setVelY(Grid &g, int x, int y, int vy) { g.setCellDataY(x, y, vy); }

void updateSand(Grid& grid, int x, int y)
{
    // Get the velocity
    int vy { getVelY(grid, x, y) }; 

    // Increase the velocity with ACCELERATION
    // vy = std::min(vy + ACCELERATION, MAX_VEL);
    vy = MAX_VEL;

    // Calculate how many cells the Sand should move this frame
    int steps {vy >> VEL_PRECISION_SHIFT };

    // Sub-step by trying to move vy cells downward
    int curX { x };
    int curY { y };

    for (int step { 0 }; step < std::max(1, steps); ++step) {
        // Fall down
        if (grid.isEmpty(curX, curY + 1)) {
            grid.move(curX, curY + 1, curX, curY);
            curY++;
        }
        else if (grid.getType(curX, curY + 1) == WATER) {
            // Swap sand and water
            grid.swap(curX, curY + 1 , curX, curY);
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
        }
         
        else {
            // Stop if it can't move anymore
            vy = 0;
            break;
        }
    } 

    // Write back the final velocity
    setVelY(grid, curX, curY, vy);
}
