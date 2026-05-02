// ============================================================
//  Maze.cpp
//  Definitions for the maze blueprint and grid helper functions.
// ============================================================

#include "Maze.h"

// ── Maze blueprint ───────────────────────────────────────────
// 21 rows × 21 columns.  Values: 0=dot 1=wall 2=power 3=eaten 4=ghost-house
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

// ── Direction delta tables ───────────────────────────────────
// Index:      0      1   2   3   4
//          NONE   UP  DOWN LEFT RIGHT
const int DX[5] = { 0,  0,  0, -1,  1 };
const int DY[5] = { 0, -1,  1,  0,  0 };

// ── Helper function implementations ─────────────────────────

bool stepGrid(int gx, int gy, Direction d, int& nx, int& ny) {
    nx = gx + DX[d];
    ny = gy + DY[d];

    // Horizontal tunnel: row 10 wraps left↔right
    if (nx < 0)     nx = COLS - 1;
    if (nx >= COLS) nx = 0;

    // Vertical out-of-bounds is not allowed
    if (ny < 0 || ny >= ROWS) return false;

    return true;
}

bool isWall(const int maze[ROWS][COLS], int gx, int gy) {
    // Treat anything out of bounds as a wall
    if (gx < 0 || gx >= COLS || gy < 0 || gy >= ROWS) return true;
    return maze[gy][gx] == TILE_WALL;
}

bool canStep(const int maze[ROWS][COLS], int gx, int gy, Direction d) {
    int nx, ny;
    if (!stepGrid(gx, gy, d, nx, ny)) return false;   // off map
    return !isWall(maze, nx, ny);                       // not a wall
}

int cellCX(int gx) {
    return gx * CELL + CELL / 2;
}

int cellCY(int gy) {
    return gy * CELL + CELL / 2;
}

int countDots(const int maze[ROWS][COLS]) {
    int count = 0;
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (maze[r][c] == TILE_DOT || maze[r][c] == TILE_POWER_DOT)
                count++;
    return count;
}
