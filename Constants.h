#pragma once
// ============================================================
//  Constants.h
//  All game-wide magic numbers live here.
//  Change a value here → it updates everywhere automatically.
// ============================================================

// ── Grid dimensions ─────────────────────────────────────────
static const int ROWS    = 21;   // number of maze rows
static const int COLS    = 21;   // number of maze columns
static const int CELL    = 30;   // pixel size of each grid cell

// ── Window layout ───────────────────────────────────────────
static const int PANEL_W = 210;               // right-side info panel width
static const int MAZE_W  = COLS * CELL;       // pixel width  of the maze area
static const int MAZE_H  = ROWS * CELL;       // pixel height of the maze area
static const int WIN_W   = MAZE_W + PANEL_W;  // total window width
static const int WIN_H   = MAZE_H + 48;       // total window height (extra bar at bottom)

// ── Movement speeds (pixels per frame) ──────────────────────
// Both values must divide CELL evenly so entities always
// land exactly on a cell center (no off-by-one drift).
// CELL=30 → PAC_SPEED=2  means Pac-Man takes 15 frames per cell
//         → GHOST_SPEED=1 means ghosts  take 30 frames per cell
static const int PAC_SPEED   = 2;
static const int GHOST_SPEED = 1;

// ── Maze tile values ────────────────────────────────────────
// These are the values stored in the 2-D maze array.
static const int TILE_WALL       = 1;   // solid blue wall
static const int TILE_DOT        = 0;   // small pellet (+10 pts)
static const int TILE_POWER_DOT  = 2;   // large pulsing pellet (+50 pts, scares ghosts)
static const int TILE_EATEN      = 3;   // already-eaten cell (empty floor)
static const int TILE_GHOST_HOUSE= 4;   // ghost spawn area (ghosts can walk here)

// ── Scoring ─────────────────────────────────────────────────
static const int SCORE_DOT        = 10;
static const int SCORE_POWER_DOT  = 50;
static const int SCORE_GHOST      = 200;
static const int SCORE_LEVEL_BONUS= 500;   // multiplied by level number

// ── Power-up timing ─────────────────────────────────────────
static const int POWER_DURATION = 300;   // frames that ghosts stay scared
