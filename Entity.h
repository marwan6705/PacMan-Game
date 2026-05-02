#pragma once
// ============================================================
//  Entity.h
//  Defines the data structures for moving game objects
//  (Pac-Man and ghosts) and the functions that move them.
//
//  Key idea – "tile-locked" movement:
//    Every entity always moves from the CENTER of one grid
//    cell to the CENTER of an adjacent cell.  They are never
//    "between" cells in an ambiguous way; they just slide
//    pixel-by-pixel until they arrive.
// ============================================================

#include "raylib.h"     // needs Color (used by Ghost::normalColor)
#include "Maze.h"       // needs Direction, ROWS, COLS, CELL

// ── Ghost behaviour modes ────────────────────────────────────
enum GhostMode {
    CHASE,   // ghost chases Pac-Man via BFS
    SCARED   // ghost runs away (Pac-Man ate a power dot)
};

// ── Entity ───────────────────────────────────────────────────
// Shared by both Pac-Man and each ghost.
struct Entity {
    int       gx, gy;    // current grid cell (column, row)
    int       px, py;    // pixel position of the entity's CENTER
    int       tgx, tgy;  // TARGET grid cell we are sliding toward
    Direction dir;        // direction we are currently moving
    bool      moving;     // true while sliding between two cells
};

// ── Ghost ────────────────────────────────────────────────────
// Wraps an Entity and adds ghost-specific fields.
struct Ghost {
    Entity    e;           // position / movement data
    Direction wantDir;     // direction chosen by AI (applied next tile)
    GhostMode mode;        // CHASE or SCARED
    int       scaredTimer; // frames remaining in SCARED mode
    Color     normalColor; // the ghost's own colour (Red/Pink/Cyan/Orange)
};

// ── Entity helper functions ──────────────────────────────────

// Place an entity exactly at grid cell (gx, gy) with no movement.
// Call this to spawn or respawn an entity.
void entityPlace(Entity& e, int gx, int gy);

// Tell entity 'e' to start sliding toward the neighbor in direction d.
// Returns false and does nothing if that cell is a wall or out of bounds.
bool entityStartMove(Entity& e, const int maze[ROWS][COLS], Direction d);

// Advance the entity 'speed' pixels toward its target cell center.
// Returns true the frame the entity arrives (it snaps exactly to center).
bool entitySlide(Entity& e, int speed);
