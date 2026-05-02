#pragma once
// ============================================================
//  
//  Ghost AI system. Uses Breadth-First Search (BFS) to find 
//  the shortest path to the player.
//
//  I chose BFS because it searches the grid layer by layer. 
//  This guarantees that the first time it reaches the target, 
//  it has found the shortest possible path since every step 
//  costs the same.
// ============================================================

#include "Maze.h"   

// Finds the very first direction the ghost needs to move 
// from its current position (sx, sy) to reach the target (tx, ty).
//
// Parameters:
//   maze      - the current game board (walls, paths, etc.)
//   sx, sy    - ghost's current grid position
//   tx, ty    - target's grid position (usually Pac-Man)
//
// Returns:
//   The direction to walk for the first step.
//   Returns DIR_NONE if we are already there or stuck.
Direction BFS(const int maze[ROWS][COLS],
              int sx, int sy,
              int tx, int ty);