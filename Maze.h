#pragma once
// ============================================================
//  
//  The maze template (blueprint) and helper functions for
//  working with grid coordinates and directions.
// ============================================================

#include "Constants.h"

// ── Direction enum ───────────────────────────────────────────
// Used by both Pac-Man and the ghosts to describe movement.
enum Direction {
    DIR_NONE  = 0,
    DIR_UP    = 1,
    DIR_DOWN  = 2,
    DIR_LEFT  = 3,
    DIR_RIGHT = 4
};

// ── The maze blueprint ───────────────────────────────────────
// This is never modified directly; it is copied into Game::maze
// at the start of every level so dots can be "eaten" in-game.
//
// Legend:   1 = wall        0 = dot (small pellet)
//           2 = power dot   3 = eaten (empty)   4 = ghost house
extern const int MAZE_TEMPLATE[ROWS][COLS];

// ── Direction offset tables ──────────────────────────────────
// DX[d] and DY[d] give the (col, row) delta for direction d.
// Index matches the Direction enum: 0=NONE, 1=UP, 2=DOWN, 3=LEFT, 4=RIGHT
extern const int DX[5];
extern const int DY[5];

// ── Grid helper functions ────────────────────────────────────

// Move one cell from (gx,gy) in direction d.
// Handles the horizontal tunnel wrap on row 10.
// Returns false if the destination is off the top/bottom edge.
bool stepGrid(int gx, int gy, Direction d, int& nx, int& ny);

// Returns true if cell (gx,gy) is a wall (or out of bounds).
bool isWall(const int maze[ROWS][COLS], int gx, int gy);

// Returns true if it is possible to step from (gx,gy) in direction d
// (i.e., the destination exists and is not a wall).
bool canStep(const int maze[ROWS][COLS], int gx, int gy, Direction d);

// Convert a grid column to the pixel x-coordinate of that cell's center.
int cellCX(int gx);

// Convert a grid row to the pixel y-coordinate of that cell's center.
int cellCY(int gy);

// Count how many dot/power-dot tiles exist in the given maze array.
int countDots(const int maze[ROWS][COLS]);
