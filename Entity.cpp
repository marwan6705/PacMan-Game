// ============================================================
//  Entity.cpp
//  Implementations for entity placement and tile-locked movement.
// ============================================================

#include "Entity.h"

// Place the entity at grid cell (gx, gy), centered perfectly.
// All movement fields are reset so it starts stationary.
void entityPlace(Entity& e, int gx, int gy) {
    e.gx  = gx;
    e.gy  = gy;
    e.px  = cellCX(gx);   // pixel center of that column
    e.py  = cellCY(gy);   // pixel center of that row
    e.tgx = gx;            // target = current (not moving yet)
    e.tgy = gy;
    e.dir    = DIR_NONE;
    e.moving = false;
}

// Try to begin a move from the current cell toward direction d.
// We check that the destination exists and is not a wall.
// Returns true on success, false if the move is blocked.
bool entityStartMove(Entity& e, const int maze[ROWS][COLS], Direction d) {
    if (d == DIR_NONE) return false;

    // canStep checks map bounds and wall tiles
    if (!canStep(maze, e.gx, e.gy, d)) return false;

    // Figure out which cell we are heading to
    int nx, ny;
    stepGrid(e.gx, e.gy, d, nx, ny);

    e.tgx    = nx;
    e.tgy    = ny;
    e.dir    = d;
    e.moving = true;
    return true;
}

// Move 'speed' pixels closer to the target cell center each frame.
// Returns true the frame we arrive (pixel position snaps to exact center).
bool entitySlide(Entity& e, int speed) {
    if (!e.moving) return false;

    // Pixel coordinates of the center of the target cell
    int tx = cellCX(e.tgx);
    int ty = cellCY(e.tgy);

    // ── Special case: horizontal tunnel wrap on row 10 ──────
    // When the target is on the far side of the tunnel the
    // horizontal distance looks huge; we just teleport instantly.
    int dx = tx - e.px;
    if (e.gy == 10 && (dx > MAZE_W / 2 || dx < -MAZE_W / 2)) {
        e.px     = tx;
        e.py     = ty;
        e.gx     = e.tgx;
        e.gy     = e.tgy;
        e.moving = false;
        return true;
    }

    // ── Normal sliding ───────────────────────────────────────
    // Move horizontally first, then vertically (one axis at a time).
    if (e.px != tx) {
        int step = (tx > e.px) ? speed : -speed;
        e.px += step;
        // Clamp: don't overshoot the target
        if ((step > 0 && e.px > tx) || (step < 0 && e.px < tx))
            e.px = tx;
    }
    if (e.py != ty) {
        int step = (ty > e.py) ? speed : -speed;
        e.py += step;
        if ((step > 0 && e.py > ty) || (step < 0 && e.py < ty))
            e.py = ty;
    }

    // ── Check arrival ────────────────────────────────────────
    if (e.px == tx && e.py == ty) {
        e.gx     = e.tgx;   // commit: current cell = target cell
        e.gy     = e.tgy;
        e.moving = false;
        return true;         // caller knows we just landed on a cell
    }

    return false;   // still sliding
}
