// ============================================================
// 
//  Implements the BFS algorithm using a Queue structure.
// ============================================================

#include "Pathfinding.h"
#include "Queue.h"      // Custom Queue structure

#include <cstring>      // For memset

Direction BFS(const int maze[ROWS][COLS],
              int sx, int sy,
              int tx, int ty) {

    // If we're already standing on the target, don't move.
    if (sx == tx && sy == ty) return DIR_NONE;

    // ── Arrays to keep track of the search ───────────────────
    // visited: keeps track of tiles we've already checked so we don't go in circles.
    // parentDir: keeps track of the VERY FIRST move we made to get to this branch.
    // Instead of doing backtracking from the target, we pass the first direction 
    // forward to every new tile we discover.
    bool visited[ROWS][COLS] = {};          // fills with false by default
    int  parentDir[ROWS][COLS];
    memset(parentDir, -1, sizeof(parentDir)); // set everything to -1 (meaning "empty")

    // ── Set up the starting point in the queue ───────────────
    Queue q;
    Queue_Init(&q);
    Queue_Enqueue(&q, {sx, sy});
    visited[sy][sx] = true;

    // The 4 ways we can walk
    const Direction dirs[4] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

    bool found = false;

    // ── Main search loop ─────────────────────────────────────
    while (!Queue_IsEmpty(&q) && !found) {

        // Get the next tile to look at from the front of the queue
        QPos cur;
        Queue_Dequeue(&q, &cur);

        // Try looking up, down, left, and right from this tile
        for (int d = 0; d < 4 && !found; d++) {
            int nx, ny;
            
            // Skip if the step is off the map, a wall, or already checked
            if (!stepGrid(cur.x, cur.y, dirs[d], nx, ny)) continue; 
            if (isWall(maze, nx, ny))                       continue; 
            if (visited[ny][nx])                            continue; 

            // Mark it as checked so we don't process it again
            visited[ny][nx] = true;

            // This is the trick to avoid backtracking:
            // If the current tile has no parent (meaning it's the very first step out of the start),
            // we lock in this direction. Otherwise, we just copy the parent direction from the previous tile.
            if (parentDir[cur.y][cur.x] == -1)
                parentDir[ny][nx] = d;        // lock in the first move
            else
                parentDir[ny][nx] = parentDir[cur.y][cur.x]; // pass it along to the next tile

            // Did we just step on the target?
            if (nx == tx && ny == ty) {
                found = true;
                break; // exit the loop early to save processing time
            }

            // Add this new valid tile to the queue to check its neighbors later
            Queue_Enqueue(&q, {nx, ny});
        }
    }

    // ── Final output ─────────────────────────────────────────
    if (!found || parentDir[ty][tx] == -1) {
        // If we couldn't find a path (like if Pac-Man is walled off entirely),
        // just pick the first open space so the ghost doesn't freeze in place.
        for (int d = 0; d < 4; d++)
            if (canStep(maze, sx, sy, dirs[d])) return dirs[d];
        return DIR_NONE;
    }

    // Since we dragged the first move direction forward all the way to the target,
    // we can just read the answer straight out of the array without backtracking.
    return dirs[parentDir[ty][tx]];
}