#include "sand.h"
#include <random>

constexpr int SAND_GRAVITY { 2 };   // Acceleration per frame
constexpr int SAND_MAX_VEL { 6 };  // Max velocity

static std::mt19937 rng { std::random_device{}() };
static std::uniform_int_distribution<int> directionDist{ 0, 1 };


void updateSand(Grid& grid, int x, int y)
{
    int vy { grid.getVelY(x, y) }; 

    // apply gravity
    vy = std::min(vy + SAND_GRAVITY, SAND_MAX_VEL);

    // Sub-step by trying to move vy cells downward
    int curX { x };
    int curY { y };

    int direction { directionDist(rng) ? 1 : -1 };
    for (int step { 0 }; step < vy; ++step) {
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
    grid.setVelY(curX, curY, vy);
}
