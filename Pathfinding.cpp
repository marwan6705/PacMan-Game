// ============================================================
//  Pathfinding.cpp
//  BFS ghost pathfinding – uses the Queue ADT (Lecture 4).
// ============================================================

#include "Pathfinding.h"
#include "Queue.h"      // Queue ADT from Lecture 4

#include <cstring>      // memset

Direction BFS(const int maze[ROWS][COLS],
              int sx, int sy,
              int tx, int ty) {

    // If the ghost is already on the target cell, no move needed.
    if (sx == tx && sy == ty) return DIR_NONE;

    // ── BFS bookkeeping arrays ───────────────────────────────
    // visited[r][c]   – have we added this cell to the queue yet?
    // parentDir[r][c] – which direction was the FIRST step taken
    //                   from (sx,sy) to reach cell (c,r)?
    //                   We store the first-step direction for every
    //                   cell so we can read the answer directly
    //                   from parentDir[ty][tx] at the end.
    bool visited  [ROWS][COLS] = {};          // all false
    int  parentDir[ROWS][COLS];
    memset(parentDir, -1, sizeof(parentDir)); // -1 = "not set yet"

    // ── Initialise Queue with the start cell ─────────────────
    Queue q;
    Queue_Init(&q);
    Queue_Enqueue(&q, {sx, sy});
    visited[sy][sx] = true;

    // All four directions we will try from each cell
    const Direction dirs[4] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

    bool found = false;

    // ── Main BFS loop ────────────────────────────────────────
    while (!Queue_IsEmpty(&q) && !found) {

        // Dequeue the next cell to explore
        QPos cur;
        Queue_Dequeue(&q, &cur);

        // Try all four neighbours
        for (int d = 0; d < 4 && !found; d++) {
            int nx, ny;
            if (!stepGrid(cur.x, cur.y, dirs[d], nx, ny)) continue; // off map
            if (isWall(maze, nx, ny))                       continue; // blocked
            if (visited[ny][nx])                            continue; // already seen

            visited[ny][nx] = true;

            // Record which first-step direction leads to this cell.
            // If the current cell has no parent (it IS the start), the
            // first step IS the current direction.  Otherwise inherit
            // the first-step direction from the current cell.
            if (parentDir[cur.y][cur.x] == -1)
                parentDir[ny][nx] = d;        // first move from start
            else
                parentDir[ny][nx] = parentDir[cur.y][cur.x]; // propagate

            // Did we reach the target?
            if (nx == tx && ny == ty) {
                found = true;
                break;
            }

            Queue_Enqueue(&q, {nx, ny});
        }
    }

    // ── Return result ────────────────────────────────────────
    if (!found || parentDir[ty][tx] == -1) {
        // No path found – just pick any open direction so the ghost
        // keeps moving rather than freezing.
        for (int d = 0; d < 4; d++)
            if (canStep(maze, sx, sy, dirs[d])) return dirs[d];
        return DIR_NONE;
    }

    // parentDir[ty][tx] holds the index into dirs[] of the first step
    return dirs[parentDir[ty][tx]];
}
