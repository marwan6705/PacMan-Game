// ============================================================
// 
// Maze layout + movement helpers
// ============================================================

#include "Maze.h"

// 21x21 grid map
// 0=dot, 1=wall, 2=power, 3=eaten, 4=ghost area
const int MAZE_TEMPLATE[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,2,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,2,1},
    {1,0,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,0,1},
    {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
    {1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1},
    {1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1},
    {1,1,1,1,0,1,0,1,1,4,4,4,1,1,0,1,0,1,1,1,1},
    {0,0,0,0,0,0,0,1,4,4,4,4,4,1,0,0,0,0,0,0,0},
    {1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1},
    {1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1},
    {1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,0,1},
    {1,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,2,1},
    {1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1},
    {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Direction vectors (index = direction)
const int DX[5] = { 0,  0,  0, -1,  1 }; // X movement
const int DY[5] = { 0, -1,  1,  0,  0 }; // Y movement


// ===== Movement =====

// Compute next cell
bool stepGrid(int gx, int gy, Direction d, int& nx, int& ny) {
    nx = gx + DX[d]; // move in X
    ny = gy + DY[d]; // move in Y

    // Horizontal wrap (tunnel)
    if (nx < 0)     nx = COLS - 1;
    if (nx >= COLS) nx = 0;

    // Stop if outside vertically
    if (ny < 0 || ny >= ROWS) return false;

    return true; // valid step
}

// Check wall or not
bool isWall(const int maze[ROWS][COLS], int gx, int gy) {
    // Outside = wall
    if (gx < 0 || gx >= COLS || gy < 0 || gy >= ROWS)
        return true;

    // Compare with wall value
    return maze[gy][gx] == TILE_WALL;
}

// Can move or blocked
bool canStep(const int maze[ROWS][COLS], int gx, int gy, Direction d) {
    int nx, ny;

    // Get next cell
    if (!stepGrid(gx, gy, d, nx, ny))
        return false;

    // Move only if not wall
    return !isWall(maze, nx, ny);
}


// ===== Rendering =====

// Center X of cell
int cellCX(int gx) {
    return gx * CELL + CELL / 2; // scale + center
}

// Center Y of cell
int cellCY(int gy) {
    return gy * CELL + CELL / 2;
}


// ===== Game Logic =====

// Count remaining dots
int countDots(const int maze[ROWS][COLS]) {
    int count = 0;

    // Loop over grid
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            // Count dots only
            if (maze[r][c] == TILE_DOT || maze[r][c] == TILE_POWER_DOT)
                count++;

    return count;
}