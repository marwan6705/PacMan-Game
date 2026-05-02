#pragma once
// ============================================================
//  Pathfinding.h
//  Ghost AI: finds the shortest path through the maze using
//  Breadth-First Search (BFS) – Lecture 4 (Queue ADT).
//
//  Why BFS?
//    BFS visits cells layer by layer (closest first), so the
//    first time it reaches the target it has found the
//    SHORTEST possible path.  No weights needed – every step
//    costs the same (one grid cell).
// ============================================================

#include "Maze.h"   // Direction, ROWS, COLS

// Find the FIRST direction Pac-Man should step from (sx,sy)
// to reach (tx,ty) along the shortest unblocked path.
//
// Parameters:
//   maze      – current maze array (walls, eaten cells, etc.)
//   sx, sy    – start cell  (ghost's current grid position)
//   tx, ty    – target cell (usually Pac-Man's grid position)
//
// Returns:
//   The Direction to move on the VERY FIRST step of the path.
//   Returns DIR_NONE if already at target or no path exists.
Direction BFS(const int maze[ROWS][COLS],
              int sx, int sy,
              int tx, int ty);
