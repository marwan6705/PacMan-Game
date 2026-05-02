// ============================================================
//  Game.cpp
//  Game initialisation and per-frame update logic.
// ============================================================

#include "Game.h"
#include "Pathfinding.h"   // BFS – uses Queue internally (Lec 4)
#include "Maze.h"

#include <cstring>  // memset, strcpy, strncpy

// ─────────────────────────────────────────────────────────────
//  resetLevel
//  Rebuilds the maze from the template and places all entities
//  at their starting positions.
//  fullReset=true  → new game (wipe score/lives/level too)
//  fullReset=false → next level (keep score/lives/level)
// ─────────────────────────────────────────────────────────────
void resetLevel(Game& G, bool fullReset) {

    // Copy the original maze blueprint into the working copy
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            G.maze[r][c] = MAZE_TEMPLATE[r][c];

    G.totalDots  = countDots(G.maze);
    G.dotsEaten  = 0;
    G.powerActive= false;
    G.powerTimer = 0;
    G.frame      = 0;

    // Mouth starts at 30° open and will oscillate each frame
    G.mouth        = 30.f;
    G.mouthOpening = false;
    G.pacWant      = DIR_NONE;

    // Place Pac-Man at his starting cell
    entityPlace(G.pac, 10, 16);

    // Starting grid cells for the four ghosts
    const int   gsx[4]    = { 9, 10, 10, 11 };
    const int   gsy[4]    = { 9,  9, 10, 10 };
    const Color gcols[4]  = { RED, PINK, SKYBLUE, ORANGE };

    for (int i = 0; i < 4; i++) {
        entityPlace(G.ghosts[i].e, gsx[i], gsy[i]);
        G.ghosts[i].e.dir       = DIR_UP;
        G.ghosts[i].wantDir     = DIR_UP;
        G.ghosts[i].mode        = CHASE;
        G.ghosts[i].scaredTimer = 0;
        G.ghosts[i].normalColor = gcols[i];
    }

    // Only wipe score/lives/level when starting a brand-new game
    if (fullReset) {
        G.lives = 3;
        G.score = 0;
        G.level = 1;
        Stack_Init(&G.moveHistory);   // Stack – Lec 2 & 3
    }
}

// ─────────────────────────────────────────────────────────────
//  initGame
//  Called once at program start.  Zeroes the whole struct,
//  initialises data structures, and sets the opening screen.
// ─────────────────────────────────────────────────────────────
void initGame(Game& G) {
    memset(&G, 0, sizeof(G));

    List_Init(&G.leaderboard);    // List – Lec 5

    strcpy(G.playerName, "Player");
    G.nameLen = 6;
    G.screen  = SCR_MENU;
    G.paused  = false;

    resetLevel(G, true);
}

// ─────────────────────────────────────────────────────────────
//  updateGame
//  Everything that happens in a single frame of gameplay:
//    1. Animate Pac-Man's mouth
//    2. Read player input
//    3. Move Pac-Man, pick up dots
//    4. Move ghosts (BFS AI)
//    5. Check collisions
//    6. Check win condition
// ─────────────────────────────────────────────────────────────
void updateGame(Game& G) {
    G.frame++;

    // ── 1. Mouth animation ───────────────────────────────────
    // The mouth angle oscillates between ~2° (almost closed) and ~42° (wide open).
    const float MOUTH_SPEED = 3.5f;
    if (G.mouthOpening) {
        G.mouth += MOUTH_SPEED;
        if (G.mouth >= 42.f) G.mouthOpening = false;
    } else {
        G.mouth -= MOUTH_SPEED;
        if (G.mouth <= 2.f)  G.mouthOpening = true;
    }

    // ── 2. Player input ──────────────────────────────────────
    // We buffer the desired direction so players can press a key
    // just BEFORE the corner and Pac-Man will turn as soon as possible.
    if      (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) G.pacWant = DIR_UP;
    else if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) G.pacWant = DIR_DOWN;
    else if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) G.pacWant = DIR_LEFT;
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) G.pacWant = DIR_RIGHT;

    // ── 3. Move Pac-Man ──────────────────────────────────────
    if (!G.pac.moving) {
        // Pac-Man just landed on a cell center – try to turn or continue
        bool moved = (G.pacWant != DIR_NONE &&
                      entityStartMove(G.pac, G.maze, G.pacWant));
        if (moved)
            G.pac.dir = G.pacWant;          // turn succeeded
        else
            moved = entityStartMove(G.pac, G.maze, G.pac.dir); // keep going
        if (!moved)
            G.pac.dir = DIR_NONE;           // hit a wall, stop
    }

    // Slide Pac-Man one step toward his target cell
    bool arrived = entitySlide(G.pac, PAC_SPEED);

    if (arrived) {
        // ── Push to Stack (move history – Lec 2 & 3) ────────
        Stack_Push(&G.moveHistory, {G.pac.gx, G.pac.gy});

        // ── Pick up dot or power dot ─────────────────────────
        int& cell = G.maze[G.pac.gy][G.pac.gx];

        if (cell == TILE_DOT) {
            cell = TILE_EATEN;
            G.score += SCORE_DOT;
            G.dotsEaten++;
        }
        else if (cell == TILE_POWER_DOT) {
            cell = TILE_EATEN;
            G.score += SCORE_POWER_DOT;
            G.dotsEaten++;

            // Activate power-up: all ghosts become scared
            G.powerActive = true;
            G.powerTimer  = POWER_DURATION;
            for (int i = 0; i < 4; i++) {
                G.ghosts[i].mode        = SCARED;
                G.ghosts[i].scaredTimer = POWER_DURATION;
            }
        }
    }

    // Count down the power-up timer; switch ghosts back to CHASE when done
    if (G.powerActive && --G.powerTimer <= 0) {
        G.powerActive = false;
        for (int i = 0; i < 4; i++)
            if (G.ghosts[i].mode == SCARED)
                G.ghosts[i].mode = CHASE;
    }

    // ── 4. Move ghosts ───────────────────────────────────────
    for (int i = 0; i < 4; i++) {
        Ghost&  gh = G.ghosts[i];
        Entity& ge = gh.e;

        // Tick scared timer for this ghost
        if (gh.mode == SCARED && --gh.scaredTimer <= 0)
            gh.mode = CHASE;

        if (!ge.moving) {
            // Ghost just landed on a cell – choose next direction via BFS

            // Is this ghost still inside the ghost house?
            bool inHouse = (ge.gy >= 9 && ge.gy <= 10 &&
                            ge.gx >= 8 && ge.gx <= 12);

            if (inHouse) {
                // Just keep heading up to exit the house
                gh.wantDir = DIR_UP;
            }
            else if (gh.mode == CHASE) {
                // BFS toward Pac-Man's current cell
                gh.wantDir = BFS(G.maze, ge.gx, ge.gy,
                                 G.pac.gx, G.pac.gy);
            }
            else { // SCARED
                // BFS toward the corner that is FURTHEST from Pac-Man
                int tx = (G.pac.gx < COLS / 2) ? COLS - 2 : 1;
                int ty = (G.pac.gy < ROWS / 2) ? ROWS - 2 : 1;
                gh.wantDir = BFS(G.maze, ge.gx, ge.gy, tx, ty);
            }

            // Try: preferred direction → current direction → any open direction
            if (!entityStartMove(ge, G.maze, gh.wantDir))
                if (!entityStartMove(ge, G.maze, ge.dir)) {
                    const Direction opts[4] = {DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};
                    for (int d = 0; d < 4; d++)
                        if (entityStartMove(ge, G.maze, opts[d])) break;
                }
        }

        entitySlide(ge, GHOST_SPEED);

        // ── 5. Collision detection ───────────────────────────
        // Collision counts when both are on the same grid cell.
        if (ge.gx == G.pac.gx && ge.gy == G.pac.gy) {

            if (gh.mode == SCARED) {
                // Pac-Man eats the ghost
                G.score += SCORE_GHOST;
                entityPlace(ge, 10, 9);   // respawn ghost inside house
                gh.mode = CHASE;
            }
            else {
                // Ghost catches Pac-Man
                G.lives--;
                if (G.lives <= 0) {
                    // Record score in List leaderboard (Lec 5)
                    ScoreEntry se;
                    strncpy(se.name, G.playerName, 19);
                    se.name[19] = '\0';
                    se.score = G.score;
                    se.level = G.level;
                    List_AddScore(&G.leaderboard, se);
                    G.screen = SCR_GAMEOVER;
                    return;
                }

                // Respawn Pac-Man and all ghosts
                entityPlace(G.pac, 10, 16);
                G.pacWant = DIR_NONE;

                const int rsx[4] = {9, 10, 10, 11};
                const int rsy[4] = {9,  9, 10, 10};
                for (int j = 0; j < 4; j++) {
                    entityPlace(G.ghosts[j].e, rsx[j], rsy[j]);
                    G.ghosts[j].e.dir   = DIR_UP;
                    G.ghosts[j].wantDir = DIR_UP;
                    G.ghosts[j].mode    = CHASE;
                }
                return;  // skip win check this frame
            }
        }
    } // end ghost loop

    // ── 6. Win condition ─────────────────────────────────────
    if (G.dotsEaten >= G.totalDots) {
        G.score += SCORE_LEVEL_BONUS * G.level;
        G.level++;
        resetLevel(G, false);   // start next level, keep score/lives
    }
}
